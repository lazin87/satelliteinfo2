#ifndef IREMOTEDATASTORAGE_H
#define IREMOTEDATASTORAGE_H

#include <QString>

class IData;

class IRemoteDataStorage
{
public:
    enum ERemoteDataStorageSts
    {
        RDSSts_OK = 0
    };

    IRemoteDataStorage();
    virtual ~IRemoteDataStorage();

    virtual void push(IData const & a_crData) = 0;
    virtual void pull(IData *a_pOutData) = 0;

    virtual ERemoteDataStorageSts status() const = 0;
};

#endif // IREMOTEDATASTORAGE_H
