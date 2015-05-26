#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <cdata.h>

class IDataStorage
{
public:
    enum EDataStorageStatusCode
    {
        DS_STS_EMPTY = 0,
        DS_STS_UNEMPTY = 1,
    };

    IDataStorage();
    virtual ~IDataStorage();

    virtual bool commitData(int a_iDataTypeId, QString a_strData);
    virtual bool commitData(int a_iDataTypeId, int a_iData);
    virtual bool commitData(int a_iDataTypeId, double a_dData);
    virtual bool pushData() = 0;
    virtual bool pullData(CData & a_rOutData) = 0;
    virtual EDataStorageStatusCode status() const = 0;
};

#endif // IDATASTORAGE_H
