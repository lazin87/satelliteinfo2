#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include "idata.h"
#include "iremotedatastorage.h"

class ILocalDataStorage
{
public:
    enum ELocalDataStorageSts
    {
        DS_STS_EMPTY = 0,
        DS_STS_DATA_ARE_COMMITED = 1,
    };

    ILocalDataStorage();
    virtual ~ILocalDataStorage();

    virtual bool commitData(IData const * a_cpData) = 0;
    virtual bool pushData(IRemoteDataStorage const & m_crRemoteDataStorage) = 0;
    virtual bool pullData(IRemoteDataStorage const & m_crRemoteDataStorage) = 0;
    virtual ELocalDataStorageSts status() const = 0;
};

#endif // IDATASTORAGE_H
