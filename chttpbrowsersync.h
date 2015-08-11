#ifndef CHTTPBROWSERSYNC2_H
#define CHTTPBROWSERSYNC2_H

#include <QObject>
#include <QUrl>
#include <QEventLoop>
#include <QUrlQuery>

class QFile;
class QNetworkReply;
class QNetworkAccessManager;
class QTimer;

using PostParam_t = QPair<QString, QString>;
using PostParamsList_t = QVector<PostParam_t>;

class CHttpBrowserSync : public QObject
{
    Q_OBJECT

public:
    const int iWAIT_TIMEOUTMS = 1000;
    enum EHttpRequestType
    {
        eHttpReqINVALID,
        eHttpReqGET,
        eHttpReqPOST
    };

    CHttpBrowserSync(QObject * a_pParent = 0);
    ~CHttpBrowserSync();

    void setUrl(const QString &a_crstrUrl);
    void setEHttpReq(const EHttpRequestType &eHttpReq);
    void setHttpParams(const PostParamsList_t &a_ParamsList);
    EHttpRequestType eHttpReq() const;

    bool startProcessRequest(QString & a_strOutFileName);

private slots:
    void downloadFinished();
    void dataReadyToRead();
    void waitTimeout();

signals:
    void signalReadTimeout();

private:
    // only Q_INVOKABLE slots and methods can be call be QMetaObject::invokeMethod()
    Q_INVOKABLE  void processRequest(void * a_pIsSuccess, void * a_pLoop, void *a_strOutFileName);
    bool waitEndOfProccessing(int a_iTimeout);
    bool submitHttpRequest(QString &a_strOutFileName);
    bool isGuiThread();
    bool prepareDataOutput(QString &a_rstrName);
    void clear();
    void closeOutput();
    void startHttpRequest(EHttpRequestType a_eHttpReqType = eHttpReqGET);
    void startBrowserThread();
    void endBrowserThread();
    bool checkHttpReqParams();

    QUrl m_oUrl;
    QFile * m_pOutputFile;
    QNetworkReply * m_pReplay;
    QNetworkAccessManager * m_pNetworkAccessMngr;

    QThread * m_pBrowserThread;
    QEventLoop m_oEventLoop;

    bool m_fTimeout;
    EHttpRequestType m_eHttpReq;
    QUrlQuery m_urlDataQuery;
};

#endif // CHTTPBROWSERSYNC2_H
