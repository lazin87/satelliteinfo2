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

    explicit CGpsDataStorage(QObject *parent = 0);
    ~CGpsDataStorage();

    // IDataStorage interface
public:
    virtual bool commitData(IData const * a_cpData);
    virtual bool pushData(IRemoteDataStorage &a_crRemoteDataStorage);
    virtual bool pullData(IRemoteDataStorage const & a_crRemoteDataStorage);
    virtual ELocalDataStorageSts status() const;

signals:

public slots:

private:
    ELocalDataStorageSts m_eStatus;
    QVector<CGpsPositionData> m_aLocalDataStorage;
};

#endif // CGPSDATASTORAGE_H
