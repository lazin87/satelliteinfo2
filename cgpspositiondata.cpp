#include "cgpspositiondata.h"

const QString CGpsPositionData::s_astrCOLUMNS_NAMES[CGpsPositionData::GPS_DATA_COUNT] = {
    "TIMESTAMP",
    "LONGITUDE",
    "LATITUDE",
    "ALTITIUDE" };

CGpsPositionData::CGpsPositionData()
    : IData()
{
     std::fill_n(m_aData, CGpsPositionData::GPS_DATA_COUNT, 0);
}

CGpsPositionData::~CGpsPositionData()
{

}

bool CGpsPositionData::set(int a_iColumn, double a_dVal)
{
    bool fResult = false;

    if( (0 <= a_iColumn) && (static_cast<int>(GPS_DATA_COUNT) > a_iColumn) )
    {
        m_aData[a_iColumn] = a_dVal;
        fResult = true;
    }

    return fResult;
}



bool CGpsPositionData::set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd)
{
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
    QString strOut = QString::number(m_aData[0] );

    for(int i = 1; GPS_DATA_COUNT > i; ++i)
    {
        strOut += " " + QString::number(m_aData[i]);
    }

    return strOut;
}

QString CGpsPositionData::columnName(int a_iColumn) const
{
    QString strOut = "INVALID_COLUMN";

    if( (0 <= a_iColumn) && (GPS_DATA_COUNT > a_iColumn) )
    {
        strOut = s_astrCOLUMNS_NAMES[a_iColumn];
    }

    return strOut;
}

double CGpsPositionData::get(int a_iColumn) const
{
    return m_aData[a_iColumn];
}

int CGpsPositionData::count() const
{
    return static_cast<int>(GPS_DATA_COUNT);
}


std::ostream &operator<<(std::ostream &a_rOstream, const CGpsPositionData &a_crGpsPosData)
{
    a_rOstream << "Timestamp: " << a_crGpsPosData.m_aData[CGpsPositionData::TIMESTAMP]
               << " Long: " << a_crGpsPosData.m_aData[CGpsPositionData::LATITUDE]
               << " Lat: " << a_crGpsPosData.m_aData[CGpsPositionData::LATITUDE]
               << " Alt: " << a_crGpsPosData.m_aData[CGpsPositionData::ALTITIUDE];

    return a_rOstream;
}


QDebug operator<<(QDebug a_oQDebug, const CGpsPositionData &a_crGpsPosData)
{
    a_oQDebug << "Timestamp: " << a_crGpsPosData.m_aData[CGpsPositionData::TIMESTAMP]
               << " Long: " << a_crGpsPosData.m_aData[CGpsPositionData::LATITUDE]
               << " Lat: " << a_crGpsPosData.m_aData[CGpsPositionData::LATITUDE]
               << " Alt: " << a_crGpsPosData.m_aData[CGpsPositionData::ALTITIUDE];

    return a_oQDebug;
}
