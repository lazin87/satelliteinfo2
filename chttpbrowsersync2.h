#ifndef CHTTPBROWSERSYNC2_H
#define CHTTPBROWSERSYNC2_H

#include <QObject>
#include <QUrl>
#include <QEventLoop>

class QFile;
class QNetworkReply;
class QNetworkAccessManager;

class CHttpBrowserSync2 : public QObject
{
    Q_OBJECT

public:
    CHttpBrowserSync2(QObject * a_pParent = 0);
    ~CHttpBrowserSync2();

    bool startProcessRequest();
    void setHttpParams();
    void setUrl(const QString &a_crstrUrl);


private slots:
    void downloadFinished();
    void dataReadyToRead();

private:
    // only Q_INVOKABLE slots and methods can be call be QMetaObject::invokeMethod()
    Q_INVOKABLE  void processRequest(void * a_pIsSuccess, void * a_pLoop);
    bool submitHttpRequest();
    bool isGuiThread();
    bool prepareDataOutput(QString &a_rstrName);
    void clear();
    void closeOutput();
    void startHttpRequest();
    void startBrowserThread();
    void endBrowserThread();

    QUrl m_oUrl;
    QFile * m_pOutputFile;
    QNetworkReply * m_pReplay;
    QNetworkAccessManager * m_pNetworkAccessMngr;

    QThread * m_pBrowserThread;
    QEventLoop m_oEventLoop;
};

#endif // CHTTPBROWSERSYNC2_H
