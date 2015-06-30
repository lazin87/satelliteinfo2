#ifndef CGPSPOSITIONDATA_H
#define CGPSPOSITIONDATA_H

#include "idata.h"
#include <QString>

class CGpsPositionData : public IData
{
public:
    enum EGpsDataType
    {
        TIMESTAMP = 0,
        LONGITUDE = 1,
        LATITUDE = 2,
        ALTITIUDE = 3,

        GPS_DATA_COUNT
    };

    CGpsPositionData();
    ~CGpsPositionData();

    // IData interface
    virtual bool set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd);
    virtual QString toString();
    virtual QString columnName(int a_iColumn) const;
    virtual bool get(int a_iColumn, double & a_rdOutVal) const;

private:
    static const QString s_astrCOLUMNS_NAMES[GPS_DATA_COUNT];// = {"TIMESTAMP", "LONGITUDE", "LATITUDE", "ALTITIUDE"};
    double m_aData[GPS_DATA_COUNT];// = {0};
};

#endif // CGPSPOSITIONDATA_H
