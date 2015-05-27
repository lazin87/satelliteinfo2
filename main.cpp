#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtGui/QGuiApplication>
#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>

#include "satelliteinfo.h"
#include "cgpspositionprovider.h"


// test only
#include "cgpspositiondata.h"
#include <QDebug>
// end test only

int main(int argc, char *argv[])
{
    CGpsPositionData data;
    QVector<double> test_data;//(1.2, 1.3, 1.4);
    test_data.append(1.2);
    test_data.append(1.3);
    test_data.append(1.4);

    data.set(test_data.begin(), test_data.end() );
    qWarning() << data.toString();
    qWarning() << "Columns names:"
               << " " << data.columnName(CGpsPositionData::LONGITUDE)
               << " " << data.columnName(CGpsPositionData::ALTITIUDE)
               << " " << data.columnName(CGpsPositionData::LATITUDE);
    qWarning() << "Values:";
    for(int i = 0; CGpsPositionData::GPS_DATA_COUNT > i; ++i)
    {
        double val;
        void * pval = reinterpret_cast<void *>(&val);
        data.get(i, pval);
        qWarning() << " " << val;
    }
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
