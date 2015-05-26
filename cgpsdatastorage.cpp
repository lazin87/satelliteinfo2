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



bool CGpsDataStorage::commitData(int a_iDataTypeId, double a_dData)
{
    bool fResult = false;

    if( (0 <= a_iDataTypeId) && (GPS_DATA_COUNT > a_iDataTypeId) )
    {
        fResult = true;

        m_adValues[a_iDataTypeId] = a_dData;

        m_eStatus = DS_STS_UNEMPTY;
    }

    return fResult;
}

bool CGpsDataStorage::pushData()
{
}

bool CGpsDataStorage::pullData(CData &a_rOutData)
{
}

IDataStorage::EDataStorageStatusCode CGpsDataStorage::status() const
{
}
