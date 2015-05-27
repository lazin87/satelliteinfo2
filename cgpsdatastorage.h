#ifndef CGPSDATASTORAGE_H
#define CGPSDATASTORAGE_H

#include <QObject>

#include "idatastorage.h"

class CGpsDataStorage : public QObject, public IDataStorage
{
    Q_OBJECT
public:
    enum EGpsDataType
    {
        LONGITUDE = 0,
        LATITUDE = 1,
        ALTITIUDE = 2,

        GPS_DATA_COUNT = 3
    };

    explicit CGpsDataStorage(QObject *parent = 0);
    ~CGpsDataStorage();

    // IDataStorage interface
public:
    virtual bool commitData(IData const * a_cpData);
    virtual bool pushData();
    virtual bool pullData(IData * a_pOutData);
    virtual EDataStorageStatusCode status() const;

signals:

public slots:

private:
   // QString m_astrKeys[GPS_DATA_COUNT] = {"LONGITUDE", "LATITUDE", "ALTITIUDE"};
    double m_adValues[GPS_DATA_COUNT];// = {0};
    //QVector <double [GPS_DATA_COUNT]> dupa;

    EDataStorageStatusCode m_eStatus;
};

#endif // CGPSDATASTORAGE_H
