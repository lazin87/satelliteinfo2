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



void CHttpDataStorage::push(const std::unique_ptr<IData> &a_crData)
{
    PostParamsList_t postParamList;
    const int iCOLUMN_COUNT = a_crData->count();

    for(int i = 0; iCOLUMN_COUNT > i; ++i)
    {
        PostParam_t postParam( a_crData->columnName(i), QString::number(a_crData->get(i) ) );
        postParamList.append(postParam);
    }

    m_oHttpBrowser.submitForm(strTARGET_URL, postParamList);
}

void CHttpDataStorage::pull(QVector<IData> &a_rData)
{
}

IRemoteDataStorage::ERemoteDataStorageSts CHttpDataStorage::status() const
{
    return RDSSts_INVALID;
}
