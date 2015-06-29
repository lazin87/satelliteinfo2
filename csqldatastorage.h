#ifndef CSQLDATASTORAGE_H
#define CSQLDATASTORAGE_H

#include "iremotedatastorage.h"

#include <QtSql>


class CSqlDataStorage : public IRemoteDataStorage
{
public:
    CSqlDataStorage();
    ~CSqlDataStorage();

    // IRemoteDataStorage interface
public:
    virtual void push(QVector<IData> const & a_crData);
    virtual void pull(QVector<IData> & a_rData);
    virtual ERemoteDataStorageSts status() const;

private:
    QSqlDatabase m_oSqlDatabase;
};

#endif // CSQLDATASTORAGE_H
