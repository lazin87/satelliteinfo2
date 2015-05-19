#include "satelliteinfo.h"
#include <QTimer>
#include <QDebug>

SatelliteModel::SatelliteModel(QObject *a_pParent)
    : QAbstractListModel(a_pParent)
    , m_pSource(0)
    , m_fComponentCompleted(false)
    , m_fRunning(false)
    , m_fRunningRequested(false)
    , m_fDemo(false)
    , m_fIsSingle(false)
    , m_fSingleRequestServed(false)
{
    m_pSource = QGeoSatelliteInfoSource::createDefaultSource(this);
    if(!m_fDemo && (0 == m_pSource) )
    {
        qWarning() << "No satellite data source found. Changing to demo mode.";
        m_fDemo = true;
    }

    if(!m_fDemo)
    {
        m_pSource->setUpdateInterval(iUPDATE_INTERVAL);

        connect( m_pSource, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>) )
               , this, SLOT(satellitesInViewUpdated(QList<QGeoSatelliteInfo>) )
               );
        connect( m_pSource, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>) )
               , this, SLOT(satellitesInUseUpdated(QList<QGeoSatelliteInfo>) )
               );
        connect( m_pSource, SIGNAL(error(QGeoSatelliteInfoSource::Error) )
               , this, SLOT(error(QGeoSatelliteInfoSource::Error) )
               );
    }

    if(m_fDemo)
    {
        m_pTimer = new QTimer(this);
        connect( m_pTimer, SIGNAL(timeout() )
               , this, SLOT(updateDemoData() )
               );
        m_pTimer->start(iUPDATE_INTERVAL);
    }
}

SatelliteModel::~SatelliteModel()
{

}

int SatelliteModel::rowCount(const QModelIndex &a_rParent) const
{
    Q_UNUSED(a_rParent);
    int iResult = -1;

    if( !m_fDemo && (0 == m_pSource) )
    {
        iResult = 0;
    }
    else
    {
        iResult = m_knownSatelliteIds.count();
    }

    return iResult;
}

QVariant SatelliteModel::data(const QModelIndex &a_rIndex, int a_iRole) const
{
    if(!m_fDemo && (0 == m_pSource) )
    {
        return QVariant();
    }

    if( !a_rIndex.isValid() || (0 > a_rIndex.row() ) )
    {
        return QVariant();
    }

    if( a_rIndex.row() >= m_knownSatellites.count() )
    {
        qWarning() << "SatelliteModel: Index out bound";
        return QVariant();
    }

    const QGeoSatelliteInfo & rInfo = m_knownSatellites.at(a_rIndex.row() );
    switch(a_iRole)
    {
    case IdentifierRole:
        return rInfo.satelliteIdentifier();

    case InUseRole:
        return m_satellitesInUse.contains(rInfo.satelliteIdentifier() );

    case SignalStrengthRole:
        return rInfo.signalStrength();

    case ElevationRole:
        if( !rInfo.hasAttribute(QGeoSatelliteInfo::Elevation) )
        {
            return QVariant();
        }
        return rInfo.attribute(QGeoSatelliteInfo::Elevation);

    case AzimuthRole:
        if(!rInfo.hasAttribute(QGeoSatelliteInfo::Azimuth) )
        {
            return QVariant();
        }
        return rInfo.attribute(QGeoSatelliteInfo::Azimuth);
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> SatelliteModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;

    roleNames.insert(IdentifierRole, "satelliteIdentifier");
    roleNames.insert(InUseRole, "isInUse");
    roleNames.insert(SignalStrengthRole, "signalStrength");
    roleNames.insert(ElevationRole, "elevation");
    roleNames.insert(AzimuthRole, "azimuth");

    return roleNames;
}

void SatelliteModel::classBegin() {}

void SatelliteModel::componentComplete()
{
    m_fComponentCompleted = true;
    if(m_fRunningRequested)
    {
        setRunning(true);
    }
}

bool SatelliteModel::running() const
{
    return m_fRunning;
}

void SatelliteModel::setRunning(bool a_fIsActive)
{
    if( (0 == m_pSource) && !m_fDemo)
    {
        return;
    }

    if(!m_fComponentCompleted)
    {
        m_fRunningRequested = a_fIsActive;
        return;
    }

    if(m_fRunning == a_fIsActive)
    {
        return;
    }

    m_fRunning = a_fIsActive;

    if(m_fRunning)
    {
        clearModel();

        if(m_fDemo)
        {
            m_pTimer->start(2000);
        }
        else if (isSingleRequest() )
        {
            m_pSource->requestUpdate(10000);
        }
        else
        {
            m_pSource->startUpdates();
        }

        if(m_fDemo)
        {
            m_fSingleRequestServed = false;
        }
    }
    else
    {
        if(m_fDemo)
        {
            m_pTimer->stop();
        }
        else if(!isSingleRequest() )
        {
            m_pSource->stopUpdates();
        }
    }

    Q_EMIT runningChanged();
}

bool SatelliteModel::isSingleRequest() const
{
    return m_fIsSingle;
}

void SatelliteModel::setSingleRequest(bool a_fSingle)
{
    if(running() )
    {
        qWarning() << "Cannot change single request mode while running";
        return;
    }

    if(a_fSingle != m_fIsSingle)
    {
        m_fIsSingle = a_fSingle;
        emit singleRequestChanged();
    }
}

int SatelliteModel::entryCount() const
{
    return m_knownSatellites.count();
}

bool SatelliteModel::canProvideSatelliteInfo() const
{
    return !m_fDemo;
}

void SatelliteModel::clearModel()
{
    beginResetModel();
    m_knownSatelliteIds.clear();
    m_knownSatellites.clear();
    m_satellitesInUse.clear();
    endResetModel();
}

void SatelliteModel::updateDemoData()
{
    static bool flag = true;
    QList<QGeoSatelliteInfo> satellites;

    if(flag)
    {
        for(int i = 0; 5 > i; ++i)
        {
            QGeoSatelliteInfo info;
            info.setSatelliteIdentifier(i);
            info.setSignalStrength(20 + 20 * i);
            satellites.append(info);
        }
    }
    else
    {
        for(int i = 0; 9 > i; ++i)
        {
            QGeoSatelliteInfo info;
            info.setSatelliteIdentifier(i*2);
            info.setSignalStrength(20 + 10 * i);
            satellites.append(info);
        }
    }

    satellitesInViewUpdated(satellites);

    flag ? satellitesInUseUpdated(QList<QGeoSatelliteInfo>() << satellites.at(2) )
         : satellitesInUseUpdated(QList<QGeoSatelliteInfo>() << satellites.at(3) );

    emit errorFound(flag);

    if(isSingleRequest() && !m_fSingleRequestServed)
    {
        m_fSingleRequestServed = true;
        setRunning(false);
    }
}

void SatelliteModel::error(QGeoSatelliteInfoSource::Error a_error)
{
    emit errorFound( static_cast<int>(a_error) );
}

inline bool operator<(const QGeoSatelliteInfo & a_rA, const QGeoSatelliteInfo & a_rB)
{
    return a_rA.satelliteIdentifier() < a_rB.satelliteIdentifier();
}

void SatelliteModel::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &a_rInfos)
{
    if( !running() )
    {
        return;
    }
    qWarning() << "satellitesInViewUpdated";

    int iOldEntryCount = m_knownSatellites.count();

    // temp
    //beginResetModel();
    //m_satellitesInUse.clear();
    // end temp

    QSet<int> satelliteIdsInUpdate;
    foreach(const QGeoSatelliteInfo & rInfo, a_rInfos)
    {
        qWarning() << "inView: " << rInfo.satelliteIdentifier() << rInfo.signalStrength();
        satelliteIdsInUpdate.insert(rInfo.satelliteIdentifier() );
        // temp
        //m_satellitesInUse.insert(rInfo.satelliteIdentifier() );
        // end temp
    }
    // temp
    //endResetModel();
    // end temp

    QSet<int> toBeRemoved = m_knownSatelliteIds - satelliteIdsInUpdate;

    m_knownSatellites = a_rInfos;

    std::sort(m_knownSatellites.begin(), m_knownSatellites.end() );
    m_satellitesInUse -= toBeRemoved;

    m_knownSatelliteIds = satelliteIdsInUpdate;

    if(iOldEntryCount != m_knownSatellites.count() )
    {
        emit entryCountChanged();
    }
}

void SatelliteModel::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &a_rInfos)
{
    if(!m_fRunning)
    {
        return;
    }

    beginResetModel();
    m_satellitesInUse.clear();

    foreach(const QGeoSatelliteInfo &rInfo, a_rInfos)
    {
        qWarning() << "InUSE: " << rInfo.satelliteIdentifier() << rInfo.signalStrength();
        m_satellitesInUse.insert(rInfo.satelliteIdentifier() );
    }

    endResetModel();
}

