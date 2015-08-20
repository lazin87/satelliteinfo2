#ifndef CGPSDATASTORAGE_H
#define CGPSDATASTORAGE_H

#include <QObject>
#include <QVector>

#include "ilocaldatastorage.h"
#include "cgpspositiondata.h"

class CGpsDataStorage : public QObject, public ILocalDataStorage
{
    Q_OBJECT
public:
    static const int iSLOT_COUNT = 2;

    explicit CGpsDataStorage(QObject *parent = 0);
    ~CGpsDataStorage();

    // IDataStorage interface
public:
    virtual bool commitData(IData const * a_cpData);
    bool commitData(const CGpsPositionData & a_crGpsData);
    virtual bool pushData(IRemoteDataStorage &a_crRemoteDataStorage);
    virtual bool pullData(IRemoteDataStorage const & a_crRemoteDataStorage);
    virtual ELocalDataStorageSts status() const;
    virtual int count() const;
    virtual bool isEmpty() const;

signals:

public slots:

private:
    int getNextAvailableSlot() const;

    int m_iUnlockedSlot;

    ELocalDataStorageSts m_eStatus;
    QVector<CGpsPositionData> m_aLocalDataStorage[iSLOT_COUNT];
};

#endif // CGPSDATASTORAGE_H
