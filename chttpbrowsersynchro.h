#ifndef CHTTPBROWSERSYNCHRO_H
#define CHTTPBROWSERSYNCHRO_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

#define DEF_OPENTIMEOUT     8000 /* ms */
#define DEF_READTIMEOUT     8000 /* ms */
#define DEF_BUFFERSIZE      (1024*1204) /* = 1 MB */

class CHttpBrowserSynchro : public QObject
{
    Q_OBJECT

public:
    CHttpBrowserSynchro(QObject * a_pParent = 0);
    ~CHttpBrowserSynchro();

    void endSocketThread();
    virtual void close();
    QNetworkReply::NetworkError error() const;

    bool open(qint64 a_i64Offset = 0);
    bool isGuiThread();
    void resetReadFails();
    bool waitForConnect(int a_iTimeOutms, QNetworkAccessManager *a_pManager);
    int response() const;

public slots:
    void slotOpen(void *a_pReturnSuccess, void *a_pLoop, qint64 a_i64Offset = 0);

protected:
    bool workerOpen(qint64 a_i64Offset = 0);
    void clear();
    QNetworkReply::NetworkError responseToError(int a_iResponse) const;

protected:
    const QNetworkProxy * m_pNetworkProxy;
    QThread * m_pSocketThread;
    QEventLoop m_oEventLoop;
    QNetworkReply * m_pReplay;
    QUrl m_oUrl;
    qint64 m_i64Pos;
    QNetworkReply::NetworkError m_oNetworkError;
    qint64 m_i64BufferSize;
    int m_iOpenTimeoutMs;
    int m_iReadTimeoutMs;
    qint64 m_i64Size;
    int m_iResponse;
    bool m_fHaveSize;
    bool m_fReadTimeout;
    int m_iReadFails;
    QString m_strResponse;
    QString m_strContentType;
    QDateTime m_oLastModified;

private slots:
    void slotProxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator);
    void slotAuthenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator);
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void slotWaitTimeout();

signals:
    void signalReadTimeout();
};

#endif // CHTTPBROWSERSYNCHRO_H
