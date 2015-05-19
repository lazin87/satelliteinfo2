#ifndef SATELLITEINFO_H
#define SATELLITEINFO_H

#include <QAbstractListModel>
#include <QQmlParserStatus>
#include <QSet>
#include <QtQml/qqml.h>
#include <QtQml/QQmlParserStatus>
#include <QtPositioning/QGeoSatelliteInfoSource>

QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QGeoSatelliteInfoSource)

class SatelliteModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool satelliteInfoAvailable READ canProvideSatelliteInfo NOTIFY canProvideSatelliteInfoChanged)
    Q_PROPERTY(int entryCount READ entryCount NOTIFY entryCountChanged)
    Q_PROPERTY(bool singleRequestMode READ isSingleRequest WRITE setSingleRequest NOTIFY singleRequestChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit SatelliteModel(QObject *a_pParent = 0);
    ~SatelliteModel();

    const int iUPDATE_INTERVAL = 3000;

    enum
    {
        IdentifierRole = Qt::UserRole + 1,
        InUseRole,
        SignalStrengthRole,
        ElevationRole,
        AzimuthRole
    };

    // from QAbstracListModel
    int rowCount(const QModelIndex & a_rParent) const;
    QVariant data(const QModelIndex &a_rIndex, int a_iRole) const;
    QHash<int, QByteArray> roleNames() const;

    //From QQmlParserStatus
    void classBegin();
    void componentComplete();

    bool running() const;
    void setRunning(bool a_fIsActive);

    bool isSingleRequest() const;
    void setSingleRequest(bool a_fSingle);

    int entryCount() const;

    bool canProvideSatelliteInfo() const;

signals:
    void runningChanged();
    void entryCountChanged();
    void errorFound(int a_iCode);
    void canProvideSatelliteInfoChanged();
    void singleRequestChanged();

public slots:
    void clearModel();
    void updateDemoData();

private slots:
    void error(QGeoSatelliteInfoSource::Error);
    void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> & a_rInfos);
    void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> & a_rInfos);

private:
    QGeoSatelliteInfoSource * m_pSource;
    bool m_fComponentCompleted;
    bool m_fRunning;
    bool m_fRunningRequested;
    QList<QGeoSatelliteInfo> m_knownSatellites;
    QSet<int> m_knownSatelliteIds;
    QSet<int> m_satellitesInUse;
    bool m_fDemo;
    QTimer * m_pTimer;
    bool m_fIsSingle;
    bool m_fSingleRequestServed;
};

QML_DECLARE_TYPE(SatelliteModel)

#endif // SATELLITEINFO_H
