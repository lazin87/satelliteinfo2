#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include "idata.h"

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

    virtual bool commitData(IData const * a_cpData) = 0;
    virtual bool pushData() = 0;
    virtual bool pullData(IData * a_pOutData) = 0;
    virtual EDataStorageStatusCode status() const = 0;
};

#endif // IDATASTORAGE_H
