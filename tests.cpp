#include "cgpspositiondata.h"
#include <QDebug>

void unittest_CGpsPositionData()
{
    CGpsPositionData data;
    QVector<double> test_data;//(1.2, 1.3, 1.4);
    test_data.append(1.2);
    test_data.append(1.3);
    test_data.append(1.4);

    data.set(test_data.begin(), test_data.end() );
    qWarning() << data.toString();
    qWarning() << "Columns names:"
               << " " << data.columnName(CGpsPositionData::LONGITUDE)
               << " " << data.columnName(CGpsPositionData::ALTITIUDE)
               << " " << data.columnName(CGpsPositionData::LATITUDE)
               << " " << data.columnName(4)
               << " " << data.columnName(-1);
    qWarning() << "Values:";
    for(int i = -1; CGpsPositionData::GPS_DATA_COUNT >= i; ++i)
    {
        double val;
        if(data.get(i, val) )
        {
            qWarning() << " " << val;
        }
    }
}
