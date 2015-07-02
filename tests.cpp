#include "cgpspositiondata.h"
#include "chttpdatastorage.h"
#include "chttpbrowser.h"
#include "cgpsdatastorage.h"

#include <QDebug>
#include <ctime>
#include <cstdlib>
#include <QThread>


CHttpBrowser oHttpBrowser; // MUSI

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
    CHttpDataStorage oHttpDataStorage(oHttpBrowser);
    CGpsPositionData oData;

    QVector<double> test_data;
    test_data.append(1.23);
    test_data.append(1.33);
    test_data.append(1.43);
    test_data.append(1.53);

    oData.set(test_data.begin(), test_data.end() );

    oHttpDataStorage.push(oData);
}

void unittest_CHttpBrowser()
{
    const int iSUBMITS_ATTEMPS = 10;

    const QString urlPost = "http://www.cs.tut.fi/cgi-bin/run/~jkorpela/echo.cgi";
    PostParamsList_t params;
    params.append(qMakePair(QString("box"), QString("yes") ) );
    params.append(qMakePair(QString("hidden field"), QString("hidden test") ) );
    params.append(qMakePair(QString("Comments"), QString("to sa komentarze") ) );

    for(int i = 0; iSUBMITS_ATTEMPS > i; ++i)
    {
        bool fResult = oHttpBrowser.submitForm(urlPost, params);

        do
        {
            QThread::sleep(10);
            fResult = oHttpBrowser.submitForm(urlPost, params);
        }while(!fResult);
    }

}

void unittest_CGpsDataStorage_upload()
{
    const int iNUMBER_OF_DATAPOINTS = 5;
    const int iDATA_MIN = 0;
    const int iDATA_MAX = 180;

    CGpsDataStorage oGpsDataStorage;
    CHttpDataStorage oHttpDataStorage(oHttpBrowser);
    CGpsPositionData oGpsData;

    srand(time(NULL) ); // init random number generator

    for(int i = 0; iNUMBER_OF_DATAPOINTS > i; ++i)
    {
        double dRandVal = (rand() % iDATA_MAX + iDATA_MIN) / (i + 1.0);

        oGpsData.set(CGpsPositionData::TIMESTAMP, dRandVal + 1.0);
        oGpsData.set(CGpsPositionData::LONGITUDE, dRandVal + 2.0);
        oGpsData.set(CGpsPositionData::LATITUDE, dRandVal + 3.0);
        oGpsData.set(CGpsPositionData::ALTITIUDE, dRandVal + 4.0);

        oGpsDataStorage.commitData(&oGpsData);
    }

    oGpsDataStorage.pushData(oHttpDataStorage);
}
