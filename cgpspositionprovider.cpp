#include "cgpspositionprovider.h"

#include <QGeoPositionInfoSource>
#include <QDebug>
#include <QTimer>
#include <ctime>

CGpsPositionProvider::CGpsPositionProvider(QObject *a_pParent)
    : QObject(a_pParent)
    , m_dLongitude(0.0)
    , m_dLatitude(0.0)
    , m_dAltitude(0.0)
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
    return m_dLongitude;
}

double CGpsPositionProvider::latitude() const
{
    return m_dLatitude;
}

double CGpsPositionProvider::altitude() const
{
    return m_dAltitude;
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
        m_dAltitude = a_rGeoPosInfo.coordinate().altitude();
        m_dLongitude = a_rGeoPosInfo.coordinate().longitude();
        m_dLatitude = a_rGeoPosInfo.coordinate().latitude();

        //emit positionUpdated();
    }
}

void CGpsPositionProvider::generateDemoPosition()
{
    std::srand(std::time(0) );

    m_dAltitude = static_cast<double>(std::rand() % 10000);
    m_dLongitude = static_cast<double>( (std::rand() % 360) - 180 );
    m_dLatitude = static_cast<double>( (std::rand() % 180) - 90 );

    qWarning() << "Alt: " << m_dAltitude << " "
               << "Long: " << m_dLongitude << " "
               << "Lat: " << m_dLatitude;

    emit positionUpdated();
}

