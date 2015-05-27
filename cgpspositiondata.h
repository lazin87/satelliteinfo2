#ifndef CGPSPOSITIONDATA_H
#define CGPSPOSITIONDATA_H

#include "idata.h"
#include <QString>

class CGpsPositionData : public IData
{
public:
    enum EGpsDataType
    {
        LONGITUDE = 0,
        LATITUDE = 1,
        ALTITIUDE = 2,

        GPS_DATA_COUNT = 3
    };

    CGpsPositionData();
    ~CGpsPositionData();

    // IData interface
    virtual bool set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd);
    virtual QString toString();
    virtual QString columnName(int a_iRow) const;
    virtual bool get(int a_iRow, void *a_outVal) const;

private:
    static const QString s_astrCOLUMNS_NAMES[GPS_DATA_COUNT];// = {"LONGITUDE", "LATITUDE", "ALTITIUDE"};
    double m_aData[GPS_DATA_COUNT];// = {0};
};

#endif // CGPSPOSITIONDATA_H
