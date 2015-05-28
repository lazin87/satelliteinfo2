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
        m_vDataStorage.push_back(*pData);
        m_eStatus = DS_STS_DATA_ARE_COMMITED;
    }

    return fResult;
}

bool CGpsDataStorage::pushData(IRemoteDataStorage const & m_crRemoteDataStorage)
{
    bool fReturn = false;

    return fReturn;
}

bool CGpsDataStorage::pullData(const IRemoteDataStorage &m_crRemoteDataStorage)
{
    bool fReturn = false;

    return fReturn;
}

ILocalDataStorage::EDataStorageStatusCode CGpsDataStorage::status() const
{
    return DS_STS_EMPTY;
}
