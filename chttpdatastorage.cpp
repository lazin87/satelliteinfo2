#include "chttpdatastorage.h"
#include <QDebug>

CHttpDataStorage::CHttpDataStorage(CHttpBrowserSync2 &a_rHttpBrowser)
    : IRemoteDataStorage(),
      m_eDataStorageSts(RDSSts_INVALID),
      m_pHttpBrowser(&a_rHttpBrowser)
{
}

CHttpDataStorage::~CHttpDataStorage()
{

}



void CHttpDataStorage::push(IData const & a_crData)
{
    PostParamsList_t postParamList;
    const int iCOLUMN_COUNT = a_crData.count();

    for(int i = 0; iCOLUMN_COUNT > i; ++i)
    {
        PostParam_t postParam( a_crData.columnName(i), QString::number(a_crData.get(i) ) );
        postParamList.append(postParam);
    }

    m_pHttpBrowser->setUrl(strTARGET_URL);
    m_pHttpBrowser->setEHttpReq(CHttpBrowserSync2::eHttpReqPOST);
    m_pHttpBrowser->setHttpParams(postParamList);
    m_pHttpBrowser->startProcessRequest();
}

void CHttpDataStorage::pull(QVector<IData> &a_rData)
{
}

IRemoteDataStorage::ERemoteDataStorageSts CHttpDataStorage::status() const
{
    return RDSSts_INVALID;
}
