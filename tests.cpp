#include "cgpspositiondata.h"
#include "chttpdatastorage.h"

#include <QDebug>

void unittest_CGpsPositionData()
{
    CGpsPositionData data;
    QVector<double> test_data;//(1.2, 1.3, 1.4);
    test_data.append(1.2);
    test_data.append(1.3);
    test_data.append(1.4);
    test_data.append(1.5);

    data.set(test_data.begin(), test_data.end() );
    qWarning() << data.toString();
    qWarning() << "Columns names:"
               << " " << data.columnName(CGpsPositionData::TIMESTAMP)
               << " " << data.columnName(CGpsPositionData::LONGITUDE)
               << " " << data.columnName(CGpsPositionData::ALTITIUDE)
               << " " << data.columnName(CGpsPositionData::LATITUDE)
               << " " << data.columnName(5)
               << " " << data.columnName(-1);
    qWarning() << "Values:";
    for(int i = 0; CGpsPositionData::GPS_DATA_COUNT > i; ++i)
    {
        double val = data.get(i);
        qWarning() << " " << val;
    }
}

void unittest_CSqlDataStorage()
{
    CHttpDataStorage oHttpDataStorage;
    std::unique_ptr<IData> ptrData(new CGpsPositionData);

    QVector<double> test_data;
    test_data.append(1.2);
    test_data.append(1.3);
    test_data.append(1.4);
    test_data.append(1.5);

    ptrData->set(test_data.begin(), test_data.end() );

    oHttpDataStorage.push(ptrData);

    while(1);
}
