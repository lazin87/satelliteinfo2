#include "cgpspositiondata.h"

const QString CGpsPositionData::s_astrCOLUMNS_NAMES[CGpsPositionData::GPS_DATA_COUNT] = {"LONGITUDE", "LATITUDE", "ALTITIUDE"};

CGpsPositionData::CGpsPositionData()
    : IData()
{

}

CGpsPositionData::~CGpsPositionData()
{

}



bool CGpsPositionData::set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd)
{   /// poprawic typ iteratora !!!!!!!!!!
    int i = 0;

    for( auto iter = a_iterBegin
       ; (iter != a_iterEnd) && (static_cast<int>(GPS_DATA_COUNT) > i)
       ; ++iter, ++i
       )
    {
        m_aData[i] = *iter;
    }

    return GPS_DATA_COUNT == i;
}

QString CGpsPositionData::toString()
{
    QString strOut = "CGpsPositionData:";

    for(int i = 0; GPS_DATA_COUNT > i; ++i)
    {
        strOut += " " + QString::number(m_aData[i]);
    }

    return strOut;
}

QString CGpsPositionData::columnName(int a_iRow) const
{
    QString strOut = "INVALID_COLUMN";

    if( (0 <= a_iRow) && (GPS_DATA_COUNT > a_iRow) )
    {
        strOut = s_astrCOLUMNS_NAMES[a_iRow];
    }

    return strOut;
}

bool CGpsPositionData::get(int a_iRow, void * a_outVal) const
{
    bool fResult = false;

    if( (0 <= a_iRow) && (GPS_DATA_COUNT > a_iRow) && (0 != a_outVal) )
    {
        double * outVal = static_cast<double *>(a_outVal);
        *outVal = m_aData[a_iRow]; // TO CHECK
        fResult = true;
    }

    return fResult;
}
