#ifndef CGPSPOSITIONPROVIDER_H
#define CGPSPOSITIONPROVIDER_H

//#include <QtQml/QQmlParserStatus>
//#include <QtQml/qqml.h>
#include <QQmlParserStatus>

#include <QGeoPositionInfo>
#include "cgpspositiondata.h"

QT_FORWARD_DECLARE_CLASS (QGeoPositionInfoSource)
QT_FORWARD_DECLARE_CLASS (QTimer)

class CGpsPositionProvider : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(double longitude READ longitude NOTIFY positionChanged)
    Q_PROPERTY(double latitude READ latitude)
    Q_PROPERTY(double altitude READ altitude)
    Q_INTERFACES(QQmlParserStatus)

public:
    const int iUPDATE_INTERVAL_MS = 500;

    explicit CGpsPositionProvider(QObject *a_pParent = 0);
    ~CGpsPositionProvider();

    void start();
    void stop();

    double longitude() const;
    double latitude() const;
    double altitude() const;
    CGpsPositionData getGpsPosData() const;

    //From QQmlParserStatus
    void classBegin();
    void componentComplete();

signals:
    void positionChanged();

private slots:
    void updatePosition(const QGeoPositionInfo & a_rGeoPosInfo);
    void generateDemoPosition();

private:
    CGpsPositionData m_oGpsPosData;
    QString m_strSoruceName;
    QString m_strPositioningMethod;
    QGeoPositionInfoSource * m_pGeoPositionSrc;

    QTimer * m_pTimer;
};

#endif // CGPSPOSITIONPROVIDER_H
