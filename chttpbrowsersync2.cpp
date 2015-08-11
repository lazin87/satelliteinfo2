#include "chttpbrowsersync2.h"

#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>

CHttpBrowserSync2::CHttpBrowserSync2(QObject *a_pParent)
    : QObject(a_pParent)
    , m_oUrl()
    , m_pOutputFile(0)
    , m_pReplay(0)
    , m_pBrowserThread(0)
{
    m_pNetworkAccessMngr = new QNetworkAccessManager(this);

    startBrowserThread();
}

CHttpBrowserSync2::~CHttpBrowserSync2()
{
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
        qDebug() << "Is a GUI Thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::QueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, &m_oEventLoop)
                                  );

        m_oEventLoop.exec();
    }
    else
    {
        qDebug() << "Is not a GUI thread";
        QMetaObject::invokeMethod(this, "processRequest", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &fResult),
                                  Q_ARG(void *, 0)
                                  );
    }

    return fResult;
}

bool CHttpBrowserSync2::submitHttpRequest()
{
    // TO DO
    qDebug() << "Submit http req url: " << m_oUrl.toString();

    bool fResult = !m_oUrl.isEmpty();

    if(fResult)
    {
        QFileInfo fileInfo(m_oUrl.path() );
        QString strFileName = fileInfo.fileName();

        fResult = prepareDataOutput(strFileName);
    }

    if(fResult)
    {
        startHttpRequest();
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
    // TO DO
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
        qWarning() << "Filename is empty";
        a_rstrName = "result.txt";
    }

    if(QFile::exists(a_rstrName) )
    {
        qWarning() << "File will be overwritten: " << a_rstrName;
        QFile::remove(a_rstrName);
    }

    closeOutput(); // zastanowic czy nie przeniesc do clear albo reset()

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
    // TO DO
    m_oUrl = QUrl();

    // clear network replay
    if(0 != m_pReplay)
    {
        m_pReplay->deleteLater();
        m_pReplay = 0;
    }
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

void CHttpBrowserSync2::startHttpRequest()
{
    qDebug() << "Start httpRequest";

    QNetworkRequest oRequest(m_oUrl);

    oRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    m_pReplay = m_pNetworkAccessMngr->get(oRequest);

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

