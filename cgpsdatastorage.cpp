#include "cgpsdatastorage.h"

CGpsDataStorage::CGpsDataStorage(QObject *parent)
    : QObject(parent)
    , IDataStorage()
    , m_eStatus(DS_STS_EMPTY)
{

}

CGpsDataStorage::~CGpsDataStorage()
{

}



bool CGpsDataStorage::commitData(IData const * a_cpData)
{
    bool fResult = false;
/*
    if( (0 <= a_iDataTypeId) && (GPS_DATA_COUNT > a_iDataTypeId) )
    {
        fResult = true;

        m_adValues[a_iDataTypeId] = a_dData;

        m_eStatus = DS_STS_UNEMPTY;
    }
*/
    return fResult;
}

bool CGpsDataStorage::pushData()
{
    bool fReturn = false;

    return fReturn;
}

bool CGpsDataStorage::pullData(IData * a_pOutData)
{
    bool fReturn = false;

    return fReturn;
}

IDataStorage::EDataStorageStatusCode CGpsDataStorage::status() const
{
    return DS_STS_EMPTY;
}
