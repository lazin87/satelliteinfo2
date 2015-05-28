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
    virtual bool pushData(IRemoteDataStorage const & m_crRemoteDataStorage);
    virtual bool pullData(IRemoteDataStorage const & m_crRemoteDataStorage);
    virtual EDataStorageStatusCode status() const;

signals:

public slots:

private:
    EDataStorageStatusCode m_eStatus;
    QVector<CGpsPositionData> m_vDataStorage;
};

#endif // CGPSDATASTORAGE_H
