#include "chttpbrowsersync.h"

#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>

CHttpBrowserSync::CHttpBrowserSync(QObject *a_pParent)
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

CHttpBrowserSync::~CHttpBrowserSync()
{
    qDebug() << "CHttpBrowserSync2::~CHttpBrowserSync2()";
    m_oEventLoop.exit(1);
    closeOutput();
    endBrowserThread();
}

bool CHttpBrowserSync::startProcessRequest(QString &a_strOutFileName)
{
    // TO DO
    bool fResult = false;

    if(isGuiThread() )
    {
        qDebug() << "It is a GUI Thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::QueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, &m_oEventLoop),
                                  Q_ARG(void *, &a_strOutFileName)
                                  );

        m_oEventLoop.exec();
    }
    else
    {
        qDebug() << "It is not a GUI thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, 0),
                                  Q_ARG(void *, &a_strOutFileName)
                                  );
    }

    return fResult;
}

bool CHttpBrowserSync::checkHttpReqParams()
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

bool CHttpBrowserSync::submitHttpRequest(QString & a_strOutFileName)
{
    qDebug() << "Submit http req url: " << m_oUrl.toString();

    bool fResult = checkHttpReqParams();
    QString strFileName = "";

    if(fResult)
    {
        QFileInfo fileInfo(m_oUrl.path() );
        //QString strFileName = fileInfo.fileName();

        fResult = prepareDataOutput(strFileName);
    }
    else
    {
        qWarning() << "Invalid http req params";
    }

    if(fResult)
    {
        a_strOutFileName = strFileName;
        startHttpRequest(m_eHttpReq);
        fResult = waitEndOfProccessing(iWAIT_TIMEOUTMS);
    }
    else
    {
        a_strOutFileName = "";
    }

    return fResult;
}

void CHttpBrowserSync::setUrl(const QString &a_crstrUrl)
{
    qDebug() << "New url was set: " << a_crstrUrl;

    m_oUrl = QUrl(a_crstrUrl);
}

void CHttpBrowserSync::downloadFinished()
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

void CHttpBrowserSync::dataReadyToRead()
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

void CHttpBrowserSync::waitTimeout()
{
    qWarning() << "Wait timeout. Url: " << m_oUrl.toString();
    m_fTimeout = true;
    emit signalReadTimeout();
}

void CHttpBrowserSync::processRequest(void *a_pIsSuccess, void *a_pLoop, void * a_strOutFileName)
{
    bool * pIsSuccess = reinterpret_cast<bool *>(a_pIsSuccess);
    QString * strOutFileName = reinterpret_cast<QString *>(a_strOutFileName);

    *pIsSuccess = submitHttpRequest(*strOutFileName);

    if(0 != a_pLoop)
    {
        QMetaObject::invokeMethod( reinterpret_cast<QEventLoop *>(a_pLoop)
                                 , "quit"
                                 , Qt::QueuedConnection
                                 );
    }
}

bool CHttpBrowserSync::waitEndOfProccessing(int a_iTimeout)
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

bool CHttpBrowserSync::isGuiThread()
{
    bool fResult = false;

    QCoreApplication * pCoreApp = QCoreApplication::instance();

    if(NULL != pCoreApp)
    {
        fResult = QThread::currentThread() == pCoreApp->thread();
    }

    return fResult;
}

bool CHttpBrowserSync::prepareDataOutput(QString &a_rstrName)
{
    bool fResult = false;

    if(a_rstrName.isEmpty() )
    {
        const QString strDataFormat("ddMMyyyy_hhmmss_zzz");

        QString strDate = QDateTime::currentDateTime().toString(strDataFormat);

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

void CHttpBrowserSync::clear()
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

void CHttpBrowserSync::closeOutput()
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

void CHttpBrowserSync::startHttpRequest(EHttpRequestType a_eHttpReqType)
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

void CHttpBrowserSync::startBrowserThread()
{
    endBrowserThread();

    m_pBrowserThread = new QThread(this);
    moveToThread(m_pBrowserThread);
    m_pBrowserThread->start(QThread::HighestPriority);

}

void CHttpBrowserSync::endBrowserThread()
{
    if(0 != m_pBrowserThread)
    {
        m_pBrowserThread->quit();
        m_pBrowserThread->wait();

        delete m_pBrowserThread;
        m_pBrowserThread = 0;
    }
}
CHttpBrowserSync::EHttpRequestType CHttpBrowserSync::eHttpReq() const
{
    return m_eHttpReq;
}

void CHttpBrowserSync::setEHttpReq(const EHttpRequestType &eHttpReq)
{
    m_eHttpReq = eHttpReq;
}

void CHttpBrowserSync::setHttpParams(const PostParamsList_t &a_ParamsList)
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


