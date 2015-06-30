#ifndef CHTTPBROWSER_H
#define CHTTPBROWSER_H


/*
 * To download things, the object of a class chttpbrowser needs to be valid till request is ended.
 * Only one request can be started in time.
 */

#include <QObject>
#include <QVector>
#include <QUrl>
#include <QUrlQuery>

class QNetworkAccessManager;
class QNetworkReply;
class QFile;

using PostParam_t = QPair<QString, QString>;
using PostParamsList_t = QVector<PostParam_t>;

class CHttpBrowser : public QObject
{
    Q_OBJECT

public:
    CHttpBrowser(QObject *a_pParent = 0);
    ~CHttpBrowser();

    void downloadWebpage(const QString &a_Url);
    void submitForm(QString a_strTargetUrl, PostParamsList_t a_paramsList = PostParamsList_t() );

private slots:
    void downloadFinished();
    void downloadPostFinished();
    void dataReadyToRead();

private:
    bool startGetRequest();
    bool startPostRequest();//QUrl a_postUrl, PostParamsList_t const & a_pParamsList);
    void setPostParams(PostParamsList_t const & a_ParamsList);
    void clearPostParams();

    QUrl m_url;
    QNetworkAccessManager *m_pNetworkAccessMngr;
    QNetworkReply *m_pReplay;
    QFile *m_pFile;
    QUrlQuery m_postParamsList;
};

#endif // CHTTPBROWSER_H
