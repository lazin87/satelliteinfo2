#ifndef IREMOTEDATASTORAGE_H
#define IREMOTEDATASTORAGE_H

class IData;

class IRemoteDataStorage
{
public:
    IRemoteDataStorage();
    virtual ~IRemoteDataStorage();

    virtual void push(IData const * a_pData) = 0;
    virtual void pull(IData *a_pOutData) = 0;
};

#endif // IREMOTEDATASTORAGE_H
