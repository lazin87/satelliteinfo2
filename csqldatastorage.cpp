#include "csqldatastorage.h"

CSqlDataStorage::CSqlDataStorage()
{

}

CSqlDataStorage::~CSqlDataStorage()
{

}



void CSqlDataStorage::push(const IData &a_crData)
{
}

void CSqlDataStorage::pull(IData *a_pOutData)
{
}

IRemoteDataStorage::ERemoteDataStorageSts CSqlDataStorage::status() const
{
    return RDSSts_INVALID;
}
