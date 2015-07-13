#ifndef CHTTPBROWSERSYNCHRO_H
#define CHTTPBROWSERSYNCHRO_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QThread>

class CHttpBrowserSynchro : public QObject
{
    Q_OBJECT

public:
    CHttpBrowserSynchro(QObject * a_pParent = 0);
    ~CHttpBrowserSynchro();

    void endSocketThread();
    virtual void close();

    bool open(qint64 a_i64Offset = 0);

public slots:
    void slotOpen(void *a_pReturnSuccess, void *a_pLoop, qint64 a_i64Offset = 0);

private:
    const QNetworkProxy * m_pNetworkProxy;
    QThread * m_pSocketThread;
    QEventLoop m_oEventLoop;
};

#endif // CHTTPBROWSERSYNCHRO_H
