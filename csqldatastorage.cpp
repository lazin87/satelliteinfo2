#include "csqldatastorage.h"
#include <QDebug>

CHttpDataStorage::CHttpDataStorage()
    : m_eDataStorageSts(RDSSts_INVALID)
{
}

CHttpDataStorage::~CHttpDataStorage()
{

}



void CHttpDataStorage::push(const QVector<IData> &a_crData)
{
}

void CHttpDataStorage::pull(QVector<IData> &a_rData)
{
}

IRemoteDataStorage::ERemoteDataStorageSts CHttpDataStorage::status() const
{
    return RDSSts_INVALID;
}
