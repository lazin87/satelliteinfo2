#ifndef CHTTPBROWSERSYNCHRO_H
#define CHTTPBROWSERSYNCHRO_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

class CHttpBrowserSynchro : public QObject
{
    Q_OBJECT

public:
    CHttpBrowserSynchro(QObject * a_pParent = 0);
    ~CHttpBrowserSynchro();

    void endSocketThread();
    virtual void close();

    bool open(qint64 a_i64Offset = 0);
    bool isGuiThread();
    void resetReadFails();

public slots:
    void slotOpen(void *a_pReturnSuccess, void *a_pLoop, qint64 a_i64Offset = 0);

protected:
    bool workerOpen(qint64 a_i64Offset = 0);
    void clear();

protected:
    const QNetworkProxy * m_pNetworkProxy;
    QThread * m_pSocketThread;
    QEventLoop m_oEventLoop;
    QNetworkReply * m_pReplay;
    QUrl m_oUrl;

private slots:
    void slotProxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator);
    void slotAuthenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator);
};

#endif // CHTTPBROWSERSYNCHRO_H
