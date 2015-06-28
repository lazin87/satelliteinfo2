#ifndef IREMOTEDATASTORAGE_H
#define IREMOTEDATASTORAGE_H

#include <QString>

class IData;

class IRemoteDataStorage
{
public:
    enum ERemoteDataStorageSts
    {
        RDSSts_OK = 0,
        RDSSts_INVALID = 1
    };

    IRemoteDataStorage();
    virtual ~IRemoteDataStorage();

    virtual void push(QVector<IData> const & a_crData) = 0;
    virtual void pull(QVector<IData> & a_rData) = 0;

    virtual ERemoteDataStorageSts status() const = 0;
};

#endif // IREMOTEDATASTORAGE_H
