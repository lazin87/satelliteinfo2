#ifndef CHTTPBROWSERSYNC2_H
#define CHTTPBROWSERSYNC2_H

#include <QObject>

class CHttpBrowserSync2 : public QObject
{
    Q_OBJECT

public:
    CHttpBrowserSync2(const QString &a_crstrUrl, QObject * a_pParent = 0);
    ~CHttpBrowserSync2();

    void startHttpRequest(const QString &a_crstrUrl);
    void setHttpParams();

private:
    bool isGuiThread();
};

#endif // CHTTPBROWSERSYNC2_H
