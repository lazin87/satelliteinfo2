#include "chttpbrowsersynchro.h"

CHttpBrowserSynchro::CHttpBrowserSynchro(QObject *a_pParent)
    : QObject(a_pParent),
      m_pNetworkProxy(NULL),
      m_pReplay(NULL)
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
    if(m_pReplay != NULL)
    {
        m_pReplay->close();
        m_pReplay->deleteLater();
        m_pReplay = NULL;
    }
}

bool CHttpBrowserSynchro::open(qint64 a_i64Offset)
{
    bool bSuccess = false;

    if (isGuiThread() )
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

bool CHttpBrowserSynchro::isGuiThread()
{
    bool fResult = false;

    QCoreApplication *pCoreApp = QCoreApplication::instance();

    if(NULL != pCoreApp)
    {
        fResult = QThread::currentThread() == pCoreApp->thread();
    }

    return fResult;
}

void CHttpBrowserSynchro::resetReadFails()
{
    // TO DO
}

void CHttpBrowserSynchro::slotOpen(void *a_pReturnSuccess, void *a_pLoop, qint64 a_i64Offset)
{
    *(bool*)a_pReturnSuccess = workerOpen(a_i64Offset);

    if (a_pLoop != NULL)
    {
        QMetaObject::invokeMethod((QEventLoop*)a_pLoop, "quit", Qt::QueuedConnection);
    }
}

bool CHttpBrowserSynchro::workerOpen(qint64 a_i64Offset)
{
    qDebug() << "CHttpBrowserSynchro::workerOpen() offset = " << a_i64Offset;

    clear();
    resetReadFails();
    close();

    QNetworkAccessManager *pManager = new QNetworkAccessManager(this);
    if(NULL != m_pNetworkProxy)
    {
        pManager->setProxy(*m_pNetworkProxy);
    }

    connect( pManager, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*) ),
             this, SLOT(slotProxyAuthenticationRequired(QNetworkProxy,QAuthenticator*) )
           );

    connect( pManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*) ),
             this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*) )
           );

    QNetworkRequest oRequest;
    oRequest.setUrl(m_oUrl);
    oRequest.setRawHeader("User-Agent", "Qt NetworkAccess 1.3");
}

void CHttpBrowserSynchro::clear()
{
    // TO DO
}

void CHttpBrowserSynchro::slotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    // TO DO
}

void CHttpBrowserSynchro::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    // TO DO
}

