#include "csqldatastorage.h"

CSqlDataStorage::CSqlDataStorage()
{

}

CSqlDataStorage::~CSqlDataStorage()
{

}



void CSqlDataStorage::push(const QVector<IData> &a_crData)
{
}

void CSqlDataStorage::pull(QVector<IData> &a_rData)
{
}

IRemoteDataStorage::ERemoteDataStorageSts CSqlDataStorage::status() const
{
    return RDSSts_INVALID;
}
