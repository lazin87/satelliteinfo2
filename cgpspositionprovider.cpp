#include "cgpspositionprovider.h"

#include <QGeoPositionInfoSource>
#include <QDebug>
#include <QTimer>
#include <ctime>

CGpsPositionProvider::CGpsPositionProvider(QObject *a_pParent)
    : QObject(a_pParent)
    , m_strSoruceName("")
    , m_strPositioningMethod("")
    , m_pGeoPositionSrc(0)
    , m_pTimer(0)
{
    m_pGeoPositionSrc = QGeoPositionInfoSource::createDefaultSource(this);

    if(0 == m_pGeoPositionSrc)
    {
        m_pTimer = new QTimer(this);
    }

    start();
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
        if(0 != m_pTimer)
        {

            qWarning() << "No default GPSPositionSrc: demo ver start";

            connect(m_pTimer, SIGNAL(timeout() )
                   , this, SLOT(generateDemoPosition() )
                   );
            m_pTimer->start(iUPDATE_INTERVAL_MS);
        }
        else
        {
            qWarning() << "m_pTimer=0";
        }
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
        if( (0 != m_pTimer) && (m_pTimer->isActive() ) )
        {
            m_pTimer->stop();
        }
    }
}

double CGpsPositionProvider::longitude() const
{
    return m_oGpsPosData.get(CGpsPositionData::LONGITUDE);
}

double CGpsPositionProvider::latitude() const
{
    return m_oGpsPosData.get(CGpsPositionData::LATITUDE);
}

double CGpsPositionProvider::altitude() const
{
    return m_oGpsPosData.get(CGpsPositionData::ALTITIUDE);
}

CGpsPositionData CGpsPositionProvider::getGpsPosData() const
{
    return m_oGpsPosData;
}

void CGpsPositionProvider::classBegin()
{

}

void CGpsPositionProvider::componentComplete()
{

}

void CGpsPositionProvider::updatePosition(const QGeoPositionInfo &a_rGeoPosInfo)
{
    if(a_rGeoPosInfo.isValid() )
    {
        m_oGpsPosData.set(CGpsPositionData::LONGITUDE, a_rGeoPosInfo.coordinate().longitude() );
        m_oGpsPosData.set(CGpsPositionData::LATITUDE, a_rGeoPosInfo.coordinate().latitude() );
        m_oGpsPosData.set(CGpsPositionData::ALTITIUDE, a_rGeoPosInfo.coordinate().altitude() );
        m_oGpsPosData.set(CGpsPositionData::TIMESTAMP, 0.0);

        emit positionChanged();
    }
}

void CGpsPositionProvider::generateDemoPosition()
{
    std::srand(std::time(0) );

    m_oGpsPosData.set(CGpsPositionData::LONGITUDE, static_cast<double>( (std::rand() % 360) - 180 ) );
    m_oGpsPosData.set(CGpsPositionData::LATITUDE, static_cast<double>( (std::rand() % 180) - 90 ) );
    m_oGpsPosData.set(CGpsPositionData::ALTITIUDE, static_cast<double>(std::rand() % 10000) );
    m_oGpsPosData.set(CGpsPositionData::TIMESTAMP, 0.0);

    qWarning() << m_oGpsPosData;

    emit positionChanged();
}

