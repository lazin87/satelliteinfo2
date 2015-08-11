#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtGui/QGuiApplication>
#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>

#include "satelliteinfo.h"
#include "cgpspositionprovider.h"

#include "tests.cpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

   // unittest_CGpsPositionData();
   // unittest_CHttpBrowser();

   // unittest_CSqlDataStorage();
   // unittest_CGpsDataStorage_upload();

  //  unittest_CHttpSyncBrowserGET();
    unittest_CHttpSyncBrowserPOST();

    return app.exec();

    /*
    QGuiApplication app(argc, argv);
    qmlRegisterType<SatelliteModel>("Local", 1, 0, "SatelliteModel");
    qmlRegisterType<CGpsPositionProvider>("GpsPosProv", 1, 0, "CGpsPositionProvider");

    QQuickView view;
    // view.setSource(QStringLiteral("qrc:///main.qml"));
    view.setSource(QStringLiteral("qrc:///MyView.qml") );
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
#if defined(Q_OS_QNX)
    view.showFullScreen();
#else
    view.show();
#endif

    return app.exec();
    */
}
