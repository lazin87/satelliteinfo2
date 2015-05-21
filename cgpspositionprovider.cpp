#include "cgpspositionprovider.h"

#include <QGeoPositionInfoSource>
#include <QDebug>

CGpsPositionProvider::CGpsPositionProvider(QObject *a_pParent)
    : QObject(a_pParent)
    , m_dLongitude(0.0)
    , m_dLatitude(0.0)
    , m_dAltitude(0.0)
    , m_strSoruceName("")
    , m_strPositioningMethod("")
    , m_pGeoPositionSrc(0)
{
    m_pGeoPositionSrc = QGeoPositionInfoSource::createDefaultSource(this);
}

CGpsPositionProvider::~CGpsPositionProvider()
{
    stop();
}

void CGpsPositionProvider::start()
{
    if(0 != m_pGeoPositionSrc)
    {
        m_pGeoPositionSrc->setUpdateInterval(iUPDATE_INTERVAL_MS);
        m_pGeoPositionSrc->startUpdates();

        connect( m_pGeoPositionSrc, SIGNAL(positionUpdated(QGeoPositionInfo) )
               , this, SLOT(updatePosition(QGeoPositionInfo) )
               );
    }
    else
    {
        qWarning() << "No default GPSPositionSrc";
    }
}

void CGpsPositionProvider::stop()
{
    if(0 != m_pGeoPositionSrc)
    {
        m_pGeoPositionSrc->stopUpdates();
    }
    else
    {
        qWarning() << "No default GPSPositionSrc";
    }
}

void CGpsPositionProvider::updatePosition(const QGeoPositionInfo &a_rGeoPosInfo)
{
    if(a_rGeoPosInfo.isValid() )
    {
        m_dAltitude = a_rGeoPosInfo.coordinate().altitude();
        m_dLongitude = a_rGeoPosInfo.coordinate().longitude();
        m_dLatitude = a_rGeoPosInfo.coordinate().latitude();
    }
}

