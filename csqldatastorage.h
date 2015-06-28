#ifndef CSQLDATASTORAGE_H
#define CSQLDATASTORAGE_H

#include "iremotedatastorage.h"

class CSqlDataStorage : public IRemoteDataStorage
{
public:
    CSqlDataStorage();
    ~CSqlDataStorage();

    // IRemoteDataStorage interface
public:
    virtual void push(const IData &a_crData);
    virtual void pull(IData *a_pOutData);
    virtual ERemoteDataStorageSts status() const;
};

#endif // CSQLDATASTORAGE_H
