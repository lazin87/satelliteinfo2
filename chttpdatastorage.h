#ifndef CSQLDATASTORAGE_H
#define CSQLDATASTORAGE_H

#include "iremotedatastorage.h"


class CHttpDataStorage : public IRemoteDataStorage
{
public:

    CHttpDataStorage();

    ~CHttpDataStorage();

    // IRemoteDataStorage interface
public:
    virtual void push(QVector<IData> const & a_crData);
    virtual void pull(QVector<IData> & a_rData);
    virtual ERemoteDataStorageSts status() const;

private:
    ERemoteDataStorageSts m_eDataStorageSts;
};

#endif // CSQLDATASTORAGE_H
