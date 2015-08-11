#include "chttpbrowsersync2.h"

#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>

CHttpBrowserSync2::CHttpBrowserSync2(QObject *a_pParent)
    : QObject(a_pParent)
    , m_oUrl()
    , m_pOutputFile(0)
    , m_pReplay(0)
    , m_pBrowserThread(0)
    , m_fTimeout(false)
    , m_eHttpReq(eHttpReqINVALID)
{
    m_pNetworkAccessMngr = new QNetworkAccessManager(this);

    startBrowserThread();
}

CHttpBrowserSync2::~CHttpBrowserSync2()
{
    qDebug() << "CHttpBrowserSync2::~CHttpBrowserSync2()";
    m_oEventLoop.exit(1);
    closeOutput();
    endBrowserThread();
}

bool CHttpBrowserSync2::startProcessRequest()
{
    // TO DO
    bool fResult = false;

    if(isGuiThread() )
    {
        qDebug() << "It is a GUI Thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::QueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, &m_oEventLoop)
                                  );

        m_oEventLoop.exec();
    }
    else
    {
        qDebug() << "It is not a GUI thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, 0)
                                  );
    }

    return fResult;
}

bool CHttpBrowserSync2::checkHttpReqParams()
{
    qDebug() << "Check http req params";
    bool fResult = !m_oUrl.isEmpty();
    qDebug() << "Url: " << (fResult ? "OK" : "NOK");

    fResult = (eHttpReqINVALID != m_eHttpReq) ? fResult : false;
    qDebug() << "Req type: " << (fResult ? "OK" : "NOK");

    if(fResult && (eHttpReqPOST == m_eHttpReq) )
    {
        fResult = !m_urlDataQuery.isEmpty();
        qDebug() << "Data query: " << (fResult ? "OK" : "NOK");
    }

    return fResult;
}

bool CHttpBrowserSync2::submitHttpRequest()
{
    qDebug() << "Submit http req url: " << m_oUrl.toString();

    bool fResult = checkHttpReqParams();

    if(fResult)
    {
        QFileInfo fileInfo(m_oUrl.path() );
        //QString strFileName = fileInfo.fileName();
        QString strFileName = "";

        fResult = prepareDataOutput(strFileName);
    }
    else
    {
        qWarning() << "Invalid http req params";
    }

    if(fResult)
    {
        startHttpRequest(m_eHttpReq);
        fResult = waitEndOfProccessing(iWAIT_TIMEOUTMS);
    }

    return fResult;
}

void CHttpBrowserSync2::setUrl(const QString &a_crstrUrl)
{
    qDebug() << "New url was set: " << a_crstrUrl;

    m_oUrl = QUrl(a_crstrUrl);
}

void CHttpBrowserSync2::downloadFinished()
{
    m_pOutputFile->flush();
    m_pOutputFile->close();

    QVariant redirectionTarget = m_pReplay->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if(m_pReplay->error() )
    {
        qWarning() << "Download fail: " << m_oUrl.toString();
    }
    else if(!redirectionTarget.isNull() )
    {
        qWarning() << "Rederiection request. HANDLER NOT IMPLEMENTED";
    }
    else
    {

        qDebug() << "Download finished: " << m_oUrl.toString();
    }

    m_pReplay->deleteLater();
    m_pReplay = 0;
    delete m_pOutputFile;
    m_pOutputFile = 0;

    clear();
}

void CHttpBrowserSync2::dataReadyToRead()
{
    if(0 != m_pOutputFile)
    {
        qDebug() << "Data ready to read: " << m_oUrl.toString();
        m_pOutputFile->write(m_pReplay->readAll() );
    }
    else
    {
        qWarning("m_pFile == 0");
    }
}

void CHttpBrowserSync2::waitTimeout()
{
    qWarning() << "Wait timeout. Url: " << m_oUrl.toString();
    m_fTimeout = true;
    emit signalReadTimeout();
}

void CHttpBrowserSync2::processRequest(void *a_pIsSuccess, void *a_pLoop)
{
    bool * pIsSuccess = reinterpret_cast<bool *>(a_pIsSuccess);

    *pIsSuccess = submitHttpRequest();

    if(0 != a_pLoop)
    {
        QMetaObject::invokeMethod( reinterpret_cast<QEventLoop *>(a_pLoop)
                                 , "quit"
                                 , Qt::QueuedConnection
                                 );
    }
}

bool CHttpBrowserSync2::waitEndOfProccessing(int a_iTimeout)
{
    QEventLoop oEventLoop;
    QTimer *pTimer = 0;

    qDebug() << "Wait " << a_iTimeout << " ms for url: "
             << m_oUrl.toString();

    if(0 < a_iTimeout)
    {
        pTimer = new QTimer(this);
        connect(pTimer, SIGNAL(timeout() ),
                this, SLOT(waitTimeout() )
                );

        pTimer->setSingleShot(true);
        pTimer->start(a_iTimeout);

        connect(this, SIGNAL(signalReadTimeout() ),
                &oEventLoop, SLOT(quit() )
                );

        if(0 != m_pReplay)
        {
            connect(m_pReplay, SIGNAL(readyRead() ),
                    pTimer, SLOT(stop() )
                    );
        }
    }

    connect( m_pNetworkAccessMngr, SIGNAL(finished(QNetworkReply*) ),
             &oEventLoop, SLOT(quit() )
             );

    if(0 != m_pReplay)
    {
        connect(m_pReplay, SIGNAL(finished() ),
                &oEventLoop, SLOT(quit() )
                );
    }

    oEventLoop.exec();

    if(0 != pTimer)
    {
        pTimer->stop();
        delete pTimer;
        pTimer = 0;
    }

    qDebug() << "end of wait";

    return !m_fTimeout;
}

bool CHttpBrowserSync2::isGuiThread()
{
    bool fResult = false;

    QCoreApplication * pCoreApp = QCoreApplication::instance();

    if(NULL != pCoreApp)
    {
        fResult = QThread::currentThread() == pCoreApp->thread();
    }

    return fResult;
}

bool CHttpBrowserSync2::prepareDataOutput(QString &a_rstrName)
{
    bool fResult = false;

    if(a_rstrName.isEmpty() )
    {
        QString strDate = QDateTime::currentDateTime().toString();
        strDate = strDate.simplified();
        strDate = strDate.replace(' ', "_");
        strDate = strDate.replace(':', "");

        a_rstrName = "result_" + strDate + ".txt";
        qWarning() << "Filename is empty. Gen filename: " << a_rstrName;
    }

    if(QFile::exists(a_rstrName) )
    {
        qWarning() << "File will be overwritten: " << a_rstrName;
        QFile::remove(a_rstrName);
    }

    closeOutput();

    m_pOutputFile = new QFile(a_rstrName);

    fResult = m_pOutputFile->open(QIODevice::WriteOnly);

    if(!fResult)
    {
        qWarning() << "File: " << a_rstrName
                   << " open error: " << m_pOutputFile->errorString();

        delete m_pOutputFile;
        m_pOutputFile = 0;
    }

    return fResult;
}

void CHttpBrowserSync2::clear()
{
    m_oUrl = QUrl();

    // clear network replay
    if(0 != m_pReplay)
    {
        m_pReplay->deleteLater();
        m_pReplay = 0;
    }

    m_eHttpReq = eHttpReqINVALID;
    m_urlDataQuery.clear();
}

void CHttpBrowserSync2::closeOutput()
{
    if(0 != m_pOutputFile)
    {
        if(m_pOutputFile->isOpen() )
        {
            m_pOutputFile->flush();
            m_pOutputFile->close();
        }

        delete m_pOutputFile;
        m_pOutputFile = 0;
    }
}

void CHttpBrowserSync2::startHttpRequest(EHttpRequestType a_eHttpReqType)
{
    qDebug() << "Start httpRequest";

    QNetworkRequest oRequest(m_oUrl);


    oRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    if(eHttpReqGET == a_eHttpReqType)
    {
        m_pReplay = m_pNetworkAccessMngr->get(oRequest);
    }
    else if(eHttpReqPOST == a_eHttpReqType)
    {
        m_pReplay = m_pNetworkAccessMngr->post(oRequest, m_urlDataQuery.toString(QUrl::FullyEncoded).toUtf8() );
    }
    else
    {
        qWarning() << "INVALID HTTP REQ TYPE";
    }

    connect( m_pReplay, SIGNAL(readyRead() )
           , this, SLOT(dataReadyToRead() )
           );
    connect( m_pReplay, SIGNAL(finished() )
           , this, SLOT(downloadFinished() )
             );
}

void CHttpBrowserSync2::startBrowserThread()
{
    endBrowserThread();

    m_pBrowserThread = new QThread(this);
    moveToThread(m_pBrowserThread);
    m_pBrowserThread->start(QThread::HighestPriority);

}

void CHttpBrowserSync2::endBrowserThread()
{
    if(0 != m_pBrowserThread)
    {
        m_pBrowserThread->quit();
        m_pBrowserThread->wait();

        delete m_pBrowserThread;
        m_pBrowserThread = 0;
    }
}
CHttpBrowserSync2::EHttpRequestType CHttpBrowserSync2::eHttpReq() const
{
    return m_eHttpReq;
}

void CHttpBrowserSync2::setEHttpReq(const EHttpRequestType &eHttpReq)
{
    m_eHttpReq = eHttpReq;
}

void CHttpBrowserSync2::setHttpParams(const PostParamsList_t &a_ParamsList)
{
    if(!m_urlDataQuery.isEmpty() )
    {
        qDebug("Params list cleared()");
        m_urlDataQuery.clear();
    }
    qDebug() << "Params:";
    foreach(PostParam_t param, a_ParamsList)
    {
        qDebug() << "First: " << param.first
                 << "Second: " << param.second;

        m_urlDataQuery.addQueryItem(param.first, param.second);
    }
}


