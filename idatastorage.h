#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <cdata.h>

class IDataStorage
{
public:
    IDataStorage();
    virtual ~IDataStorage();

    virtual bool commitData(CData const & a_rData) = 0;
    virtual bool pushData() = 0;
    virtual bool pullData(CData & a_rOutData) = 0;
    virtual int status() = 0;
};

#endif // IDATASTORAGE_H
