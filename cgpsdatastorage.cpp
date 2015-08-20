#include "cgpsdatastorage.h"

CGpsDataStorage::CGpsDataStorage(QObject *parent)
    : QObject(parent)
    , ILocalDataStorage()
    , m_eStatus(DS_STS_EMPTY)
    , m_iUnlockedSlot(0)
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
        fResult = commitData(* pData);
    }

    return fResult;
}

bool CGpsDataStorage::commitData(const CGpsPositionData &a_crGpsData)
{
    bool fResult = false;

    m_aLocalDataStorage[m_iUnlockedSlot].push_back(a_crGpsData);
    m_eStatus = DS_STS_DATA_ARE_COMMITED;

    fResult = true;

    return fResult;
}

bool CGpsDataStorage::pushData(IRemoteDataStorage & a_crRemoteDataStorage)
{
    bool fReturn = false;
    int iPushedSlot = m_iUnlockedSlot;
    m_iUnlockedSlot = getNextAvailableSlot();

    if(m_iUnlockedSlot == iPushedSlot)
    {
        qWarning() << "THERE IS ONLY ONE SLOT!!";
    }

    foreach(CGpsPositionData oOneRecord, m_aLocalDataStorage[iPushedSlot] )
    {
        a_crRemoteDataStorage.push(oOneRecord);
    }

    return fReturn;
}

bool CGpsDataStorage::pullData(IRemoteDataStorage const & a_crRemoteDataStorage)
{
    bool fReturn = false;
    // TO DO
    return fReturn;
}

ILocalDataStorage::ELocalDataStorageSts CGpsDataStorage::status() const
{
    return DS_STS_EMPTY;
}

int CGpsDataStorage::count() const
{
    return m_aLocalDataStorage[m_iUnlockedSlot].count();
}

bool CGpsDataStorage::isEmpty() const
{
    return m_aLocalDataStorage[m_iUnlockedSlot].empty();
}

int CGpsDataStorage::getNextAvailableSlot() const
{
    int iResult = (m_iUnlockedSlot + 1) % iSLOT_COUNT;

    return iResult;
}
