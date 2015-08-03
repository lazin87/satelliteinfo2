#include "chttpbrowsersynchro.h"

#include <QTimer>

CHttpBrowserSynchro::CHttpBrowserSynchro(const QString & a_crstrUrl, QObject *a_pParent)
    : QObject(a_pParent),
      m_pNetworkProxy(NULL),
      m_pReplay(NULL),
      m_i64Pos(0),
      m_oNetworkError(QNetworkReply::NoError),
      m_i64BufferSize(0),
      m_iOpenTimeoutMs(0),
      m_iReadTimeoutMs(0),
      m_i64Size(0),
      m_iResponse(0),
      m_fHaveSize(false),
      m_fReadTimeout(false),
      m_iReadFails(0)
{
   // qRegisterMetaType<READMODE>("READMODE");        // Register our enum as metatype for invokeMethod
   // qRegisterMetaType<QByteArray*>("QByteArray*");  // Register QByteArray as metatype for invokeMethod

    clear();
    init();
    setUrl(a_crstrUrl);
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
    if(NULL != m_pSocketThread)
    {
        m_pSocketThread->quit();
        m_pSocketThread->wait();

        delete m_pSocketThread;
        m_pSocketThread = NULL;
    }
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

QNetworkReply::NetworkError CHttpBrowserSynchro::error() const
{
    return m_oNetworkError;
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
    m_iReadFails = 0;
}

bool CHttpBrowserSynchro::waitForConnect(int a_iTimeOutms, QNetworkAccessManager *a_pManager)
{
    QTimer *pTimer = NULL;
    QEventLoop oEventLoop;
    bool fReadTimeout = false;

    m_fReadTimeout = false;

    if(0 < a_iTimeOutms)
    {
        pTimer = new QTimer(this);

        connect( pTimer, SIGNAL(timeout() ),
                 this, SLOT(slotWaitTimeout() )
               );

        pTimer->setSingleShot(true);
        pTimer->start(a_iTimeOutms);

        connect( this, SIGNAL(signalReadTimeout() ),
                 &oEventLoop, SLOT(quit() )
                );
    }

    // Wait on QnetworkManager reply here
    connect( a_pManager, SIGNAL(finished(QNetworkReply*) ),
             &oEventLoop, SLOT(quit())
           );

    if(NULL != m_pReplay)
    {
        connect( m_pReplay, SIGNAL(readyRead() ),
                 &oEventLoop, SLOT(quit() )
               );
    }
    oEventLoop.exec();

    if(NULL != pTimer)
    {
        pTimer->stop();
        delete pTimer;
        pTimer = NULL;
    }

    fReadTimeout = m_fReadTimeout;
    m_fReadTimeout = false;

    return !fReadTimeout;
}

// returns response code (200, 404, etc)
int CHttpBrowserSynchro::response() const
{
    return m_iResponse;
}

void CHttpBrowserSynchro::setUrl(const QString &a_strUrl)
{
    m_oUrl = QUrl::fromEncoded(a_strUrl.toUtf8() );
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

    m_i64Pos = a_i64Offset;
    if(0 != m_i64Pos)
    {
        QByteArray data;
        QString strData("bytes=" + QString::number(m_i64Pos) + "-");

        data = strData.toLatin1();
        oRequest.setRawHeader("Range", data);
    }

    m_pReplay = pManager->get(oRequest);

    if(NULL == m_pReplay)
    {
        qDebug() << "CHttpBrowserSynchro::open(): network error";
        m_oNetworkError = QNetworkReply::UnknownNetworkError;
        return false;
    }

    m_pReplay->setReadBufferSize(m_i64BufferSize);
    connect( m_pReplay, SIGNAL(error(QNetworkReply::NetworkError) ),
             SLOT(slotError(QNetworkReply::NetworkError) )
           );
    connect( m_pReplay, SIGNAL(sslErrors(QList<QSslError>) ),
             SLOT(slotSslErrors(QList<QSslError>) )
           );

    if(!waitForConnect(m_iOpenTimeoutMs, pManager) )
    {
        qDebug() << "CHttpBrowserSynchro::open(): timeout error";
        m_oNetworkError = QNetworkReply::TimeoutError;
        return false;
    }

    if(NULL == m_pReplay)
    {
        qDebug() << "CHttpBrowserSynchro::open(): cancelled";
        m_oNetworkError = QNetworkReply::OperationCanceledError;
        return false;
    }

    if( QNetworkReply::NoError != m_pReplay->error() )
    {
        qDebug() << "CHttpBrowserSynchro::open(): error" << m_pReplay->errorString();
        m_oNetworkError = m_pReplay->error();
        return false;
    }

    m_oNetworkError = m_pReplay->error();

    m_strContentType = m_pReplay->header(QNetworkRequest::ContentTypeHeader).toString();
    m_oLastModified = m_pReplay->header(QNetworkRequest::LastModifiedHeader).toDateTime();
    m_i64Size = m_pReplay->header(QNetworkRequest::ContentLengthHeader).toULongLong();

    m_iResponse = m_pReplay->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_strResponse = m_pReplay->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    m_fHaveSize = (0 != m_i64Size);

    m_i64Size += m_i64Pos;

    if(QNetworkReply::NoError != error() )
    {
        qDebug() << "CHttpBrowserSynchro::open(): error" << error();
        return false;
    }

    m_oNetworkError = responseToError(m_iResponse);

    qDebug() << "CHttpBrowserSynchro::open(): end response" << response()
             << "error" << error() << "size" << m_i64Size;

    return (200 == response() || 206 == response() );
}

void CHttpBrowserSynchro::init()
{
    endSocketThread();

    m_pSocketThread = new QThread(this);
    moveToThread(m_pSocketThread);
    m_pSocketThread->start(QThread::HighestPriority);
}

void CHttpBrowserSynchro::clear()
{
    m_pReplay            = NULL;
    //m_offset            = 0;
    m_i64Size             = 0;
    m_fHaveSize         = false;
    m_i64Pos              = 0;
    //m_nReceivedPos      = 0;
    //m_nOverHead         = 0;
    m_iReadFails        = 0;
    m_oNetworkError      = QNetworkReply::NoError;
    m_iResponse         = 0;
    m_iOpenTimeoutMs    = DEF_OPENTIMEOUT;
    m_iReadTimeoutMs    = DEF_READTIMEOUT;
    m_fReadTimeout      = false;
    m_strContentType.clear();
    m_i64Size             = 0;
    //m_sslerrors.clear();
    m_i64BufferSize       = DEF_BUFFERSIZE;
    //m_bResetMonitor     = false;
}

QNetworkReply::NetworkError CHttpBrowserSynchro::responseToError(int a_iResponse) const
{
    QNetworkReply::NetworkError error = QNetworkReply::NoError;

    switch (a_iResponse)
    {
    case 200:
    case 206:
        break;
    case 401:
        error = QNetworkReply::AuthenticationRequiredError;
        break;
    case 403:
        error = QNetworkReply::ContentOperationNotPermittedError;
        break;
    case 404:
        error = QNetworkReply::ContentNotFoundError;
        break;
    default:
        error = QNetworkReply::UnknownNetworkError;
        break;
    }

    return error;
}

void CHttpBrowserSynchro::slotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    Q_UNUSED(proxy)
    Q_UNUSED(authenticator)

    qDebug() << "CHttpBrowserSynchro::slotProxyAuthenticationRequired()";
}

void CHttpBrowserSynchro::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply)
    Q_UNUSED(authenticator)

    qDebug() << "CHttpBrowserSynchro::slotAuthenticationRequired()";
}

void CHttpBrowserSynchro::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "CHttpBrowserSynchro::slotError()";
}

void CHttpBrowserSynchro::slotSslErrors(QList<QSslError>)
{
    qDebug() << "CHttpBrowserSynchro::slotSslErrors()";
}

void CHttpBrowserSynchro::slotWaitTimeout()
{
    m_fReadTimeout = true;  // Report timeout
    emit signalReadTimeout();
}

