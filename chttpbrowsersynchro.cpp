#include "chttpbrowsersynchro.h"

CHttpBrowserSynchro::CHttpBrowserSynchro(QObject *a_pParent)
    : QObject(a_pParent),
      m_pNetworkProxy(NULL)
{
    m_pSocketThread = new QThread(this);
    moveToThread(m_pSocketThread);
    m_pSocketThread->start(QThread::HighestPriority);
}

CHttpBrowserSynchro::~CHttpBrowserSynchro()
{
    //Ensure we exit all loops
    m_oEventLoop.exit(1);
    // Close any connection
    close();
    // End socket thread
    endSocketThread();
}

void CHttpBrowserSynchro::endSocketThread()
{

}

void CHttpBrowserSynchro::close()
{

}

bool CHttpBrowserSynchro::open(qint64 a_i64Offset)
{
    bool bSuccess = false;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        QMetaObject::invokeMethod(this, "slotOpen", Qt::QueuedConnection,
                                  Q_ARG(void *, &bSuccess),
                                  Q_ARG(void *, &m_oEventLoop),
                                  Q_ARG(qint64, a_i64Offset));
        m_oEventLoop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the open operation).
        QMetaObject::invokeMethod(this, "slotOpen", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &bSuccess),
                                  Q_ARG(void *, NULL),
                                  Q_ARG(qint64, a_i64Offset));
    }

    return bSuccess;
    // Please note that it's perfectly safe to wait on the return Q_ARG,
    // as we wait for the invokeMethod call to complete.
}

void CHttpBrowserSynchro::slotOpen(void *a_pReturnSuccess, void *a_pLoop, qint64 a_i64Offset)
{
    *(bool*)a_pReturnSuccess = workerOpen(a_i64Offset);

    if (a_pLoop != NULL)
    {
        QMetaObject::invokeMethod((QEventLoop*)a_pLoop, "quit", Qt::QueuedConnection);
    }
}

