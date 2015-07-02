#ifndef CHTTPBROWSER_H
#define CHTTPBROWSER_H


/*
 * To download things, the object of a class chttpbrowser needs to be valid till request is ended.
 * Only one request can be started in time.
 */

/*
 * Zabezpieczyc klase przed wielokrotna proba submitowania
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
    enum EHttpBrowserSts
    {
        HBSts_BUSY = 0,
        HBSts_IDLE = 1
    };

    CHttpBrowser(QObject *a_pParent = 0);
    ~CHttpBrowser();

  //  void downloadWebpage(const QString &a_Url);
    bool submitForm(QString const & a_strTargetUrl, PostParamsList_t const & a_paramsList = PostParamsList_t() );

private slots:
    void downloadFinished();
    void dataReadyToRead();

private:
    void startPostRequest();
    void setPostParams(PostParamsList_t const & a_ParamsList);
    void clearPostParams();
    bool validateSubmitedData(QString const & a_strTargetUrl, PostParamsList_t const & a_paramsList);

    QUrl m_url;
    QNetworkAccessManager *m_pNetworkAccessMngr;
    QNetworkReply *m_pReplay;
    QFile *m_pFile;
    QUrlQuery m_postUrlQuery;

    EHttpBrowserSts m_BrowserSts;
};

#endif // CHTTPBROWSER_H
