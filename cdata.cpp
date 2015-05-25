#include "cdata.h"

CData::CData()
{

}

CData::~CData()
{

}

bool CData::add(QString a_strName, QString a_strValue)
{
    bool fResult = false;

    int i = 0;
    foreach(QPair<QString, QString> param, m_aParams)
    {
        if(0 == param.first.compare(a_strName) )
        {
            m_aParams[i].second = a_strValue;
            fResult = true;
            break;
        }
        i++;
    }

    if(!fResult)
    {
        m_aParams.append(QPair<QString, QString>(a_strName, a_strValue) );
    }

    return fResult;
}

bool CData::remove(QString a_strName)
{
    bool fResult = false;

    int i = 0;
    foreach(QPair<QString, QString> param, m_aParams)
    {
        if(0 == param.first.compare(a_strName) )
        {
            m_aParams.removeAt(i);
            fResult = true;
            break;
        }
        i++;
    }

    return fResult;
}

int CData::count() const
{
    return m_aParams.count();
}

void CData::clear()
{
    m_aParams.clear();
}

