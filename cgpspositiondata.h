#ifndef CGPSPOSITIONDATA_H
#define CGPSPOSITIONDATA_H

#include "idata.h"
#include <QString>
#include <QDebug>

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
    virtual bool set(int a_iColumn, double a_dVal);
    virtual bool set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd);

    virtual QString toString();
    virtual QString columnName(int a_iColumn) const;
    virtual double get(int a_iColumn) const;
    virtual int count() const;

private:
    static const QString s_astrCOLUMNS_NAMES[GPS_DATA_COUNT];// = {"TIMESTAMP", "LONGITUDE", "LATITUDE", "ALTITIUDE"};
    double m_aData[GPS_DATA_COUNT];// = {0};

    friend std::ostream & operator<<(std::ostream & , const CGpsPositionData &);
    friend QDebug operator<< (QDebug a_oQDebug, const CGpsPositionData & a_crGpsPosData);
};

#endif // CGPSPOSITIONDATA_H

std::ostream & operator<<(std::ostream& a_rOstream, const CGpsPositionData & a_crGpsPosData);
QDebug operator<< (QDebug a_oQDebug, const CGpsPositionData & a_crGpsPosData);
