#include "chttpdatastorage.h"
#include <QDebug>

CHttpDataStorage::CHttpDataStorage(QObject *a_pParent)
    : IRemoteDataStorage(a_pParent),
      m_eDataStorageSts(RDSSts_INVALID),
      m_oHttpBrowser(this)
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
