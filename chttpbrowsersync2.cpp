#include "chttpbrowsersync2.h"

#include <QCoreApplication>
#include <QThread>

CHttpBrowserSync2::CHttpBrowserSync2(const QString &a_crstrUrl, QObject *a_pParent)
{

}

CHttpBrowserSync2::~CHttpBrowserSync2()
{

}

void CHttpBrowserSync2::startHttpRequest(const QString &a_crstrUrl)
{
    // TO DO
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

