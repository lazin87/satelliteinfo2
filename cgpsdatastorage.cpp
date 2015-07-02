#include "cgpsdatastorage.h"

CGpsDataStorage::CGpsDataStorage(QObject *parent)
    : QObject(parent)
    , ILocalDataStorage()
    , m_eStatus(DS_STS_EMPTY)
{

}

CGpsDataStorage::~CGpsDataStorage()
{

}



bool CGpsDataStorage::commitData(IData const * a_cpData)
{
    bool fResult = false;

    if(0 != a_cpData)
    {
        const CGpsPositionData * pData = dynamic_cast<const CGpsPositionData *>(a_cpData);
        m_aLocalDataStorage.push_back(*pData);
        m_eStatus = DS_STS_DATA_ARE_COMMITED;

        fResult = true;
    }

    return fResult;
}

bool CGpsDataStorage::pushData(IRemoteDataStorage & a_crRemoteDataStorage)
{
    bool fReturn = false;

    foreach(CGpsPositionData oOneRecord, m_aLocalDataStorage)
    {
        a_crRemoteDataStorage.push(oOneRecord);
    }

    return fReturn;
}

bool CGpsDataStorage::pullData(IRemoteDataStorage const & a_crRemoteDataStorage)
{
    bool fReturn = false;

    return fReturn;
}

ILocalDataStorage::ELocalDataStorageSts CGpsDataStorage::status() const
{
    return DS_STS_EMPTY;
}
