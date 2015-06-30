#include "chttpbrowser.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QVariant>
#include <QUrlQuery>
#include <QDebug>


CHttpBrowser::CHttpBrowser(QObject *a_pParent)
    : QObject(a_pParent)
    , m_pNetworkAccessMngr(0)
    , m_pReplay(0)
    , m_pFile(0)
{
    qDebug("ctor");
    m_pNetworkAccessMngr = new QNetworkAccessManager(this);
}

CHttpBrowser::~CHttpBrowser()
{
    qDebug("dest");
}

bool CHttpBrowser::startGetRequest()
{
    bool fResult = false;

        qDebug("Start GET request");
        m_pReplay = m_pNetworkAccessMngr->get(QNetworkRequest(m_url) );
        connect( m_pReplay, SIGNAL(finished() )
                 , this, SLOT(downloadFinished() )
                 );
        connect( m_pReplay, SIGNAL(readyRead() )
                 , this, SLOT(dataReadyToRead() )
                 );

        fResult = true;

    return fResult;
}

bool CHttpBrowser::startPostRequest()//QUrl a_postUrl, const PostParamsList_t &a_pParamsList)
{
    bool fResult = false;

    {
        qDebug("Start POST request");
        QNetworkRequest request(m_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");

        m_pReplay = m_pNetworkAccessMngr->post(request, m_postParamsList.toString(QUrl::FullyEncoded).toUtf8() );

        connect( m_pReplay, SIGNAL(finished() )
                 , this, SLOT(downloadPostFinished() )
                 );
        connect( m_pReplay, SIGNAL(readyRead() )
                 , this, SLOT(dataReadyToRead() )
                 );
        fResult = true;
    }

    return fResult;
}

void CHttpBrowser::setPostParams(const PostParamsList_t &a_ParamsList)
{
    if(!m_postParamsList.isEmpty() )
    {
        qDebug("Params list cleared()");
        m_postParamsList.clear();
    }

    foreach(PostParam_t param, a_ParamsList)
    {
        m_postParamsList.addQueryItem(param.first, param.second);
    }
}

void CHttpBrowser::downloadWebpage(const QString & a_Url)
{
    m_url = QUrl(a_Url);
    qDebug("Start download: %s", m_url.toString().toStdString().c_str() );
    QFileInfo fileInfo(m_url.path() );
    QString fileName = fileInfo.fileName();

    if(fileName.isEmpty() )
    {
        qWarning("Filename empty");
        fileName = "index.html";
    }

    if(QFile::exists(fileName) )
    {
        qDebug("File %s is overwritten", fileName.toStdString().c_str() );
        QFile::remove(fileName);
    }

    m_pFile = new QFile(fileName);
    if(!m_pFile->open(QIODevice::WriteOnly) )
    {
        qWarning("File: %s open error: %s", fileName.toStdString().c_str()
                                     , m_pFile->errorString().toStdString().c_str() );
        delete m_pFile;
        m_pFile = 0;
        return;
    }

    startGetRequest();
}

void CHttpBrowser::submitForm(QString a_strTargetUrl, PostParamsList_t a_paramsList)
{
    m_url = QUrl(a_strTargetUrl);
    if(!a_paramsList.isEmpty() )
    {
        qDebug("Set new params list");
        setPostParams(a_paramsList);
    }

    if(  m_url.isEmpty()
      || !(m_url.isValid() )
      )
    {
        qWarning("Wrong url is set");
        return;
    }

    if(m_postParamsList.isEmpty() )
    {
        qWarning("Empty param list");
        return;
    }

    qDebug("Submit form: %s", m_url.toString().toStdString().c_str() );
    QFileInfo fileInfo(m_url.path() );
    QString fileName = fileInfo.fileName();

    if(fileName.isEmpty() )
    {
        qWarning("Filename empty");
        fileName = "result.txt";
    }

    if(QFile::exists(fileName) )
    {
        qDebug("File %s is overwritten", fileName.toStdString().c_str() );
        QFile::remove(fileName);
    }

    m_pFile = new QFile(fileName);
    if(!m_pFile->open(QIODevice::WriteOnly) )
    {
        qWarning("File: %s open error: %s", fileName.toStdString().c_str()
                                     , m_pFile->errorString().toStdString().c_str() );
        delete m_pFile;
        m_pFile = 0;
        return;
    }

    startPostRequest();
}

void CHttpBrowser::downloadFinished()
{
    m_pFile->flush();
    m_pFile->close();

    QVariant redirectionTarget = m_pReplay->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if(m_pReplay->error() )
    {
        qWarning("Download fail: %s", m_pReplay->errorString().toStdString().c_str() );
        m_pFile->remove();
    }
    else if(!redirectionTarget.isNull() )
    {
        QUrl newUrl = m_url.resolved(redirectionTarget.toUrl() );

        qDebug("Redirection %s to %s", m_url.toString().toStdString().c_str()
                                   , newUrl.toString().toStdString().c_str() );
/*
        m_url = newUrl;
        m_pReplay->deleteLater();
        m_pFile->open(QIODevice::WriteOnly);
        m_pFile->resize(0);
        startRequest();
*/
        return;
    }
    else
    {
        qDebug("Downolad finished");
    }

    m_pReplay->deleteLater();
    m_pReplay = 0;
    delete m_pFile;
    m_pFile = 0;
    //exit(EXIT_SUCCESS);
}

void CHttpBrowser::downloadPostFinished()
{
    qDebug("donload post finished");
    m_pFile->flush();
    m_pFile->close();

    QVariant redirectionTarget = m_pReplay->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if(m_pReplay->error() )
    {
        qWarning("Download fail: %s", m_pReplay->errorString().toStdString().c_str() );
        m_pFile->remove();
    }
    else if(!redirectionTarget.isNull() )
    {
        QUrl newUrl = m_url.resolved(redirectionTarget.toUrl() );

        qDebug("Redirection %s to %s", m_url.toString().toStdString().c_str()
                                   , newUrl.toString().toStdString().c_str() );

        m_url = newUrl;
        m_pReplay->deleteLater();
        m_pFile->open(QIODevice::WriteOnly);
        m_pFile->resize(0);
        startPostRequest();

        return;
    }
    else
    {
        qDebug("Downolad finished");
    }

    m_pReplay->deleteLater();
    m_pReplay = 0;
    delete m_pFile;
    m_pFile = 0;
}

void CHttpBrowser::dataReadyToRead()
{
    if(0 != m_pFile)
    {
        qDebug("Data ready to read");
        m_pFile->write(m_pReplay->readAll() );
    }
    else
    {
        qWarning("m_pFile == 0");
    }
}

