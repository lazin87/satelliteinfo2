#ifndef CGPSPOSITIONPROVIDER_H
#define CGPSPOSITIONPROVIDER_H

#include <QtQml/QQmlParserStatus>
#include <QGeoPositionInfo>

class QGeoPositionInfoSource;

class CGpsPositionProvider : public QQmlParserStatus, QObject
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    // zdefiniowac q_property

public:
    const int iUPDATE_INTERVAL_MS = 500;

    explicit CGpsPositionProvider(QObject *a_pParent = 0);
    ~CGpsPositionProvider();

    void start();
    void stop();

private slots:
    void updatePosition(const QGeoPositionInfo & a_rGeoPosInfo);

private:

    double m_dLongitude;
    double m_dLatitude;
    double m_dAltitude;
    QString m_strSoruceName;
    QString m_strPositioningMethod;
    QGeoPositionInfoSource * m_pGeoPositionSrc;
};

#endif // CGPSPOSITIONPROVIDER_H
