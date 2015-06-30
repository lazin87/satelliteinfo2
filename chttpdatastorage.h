#ifndef CSQLDATASTORAGE_H
#define CSQLDATASTORAGE_H


#include "iremotedatastorage.h"
#include "chttpbrowser.h"


class CHttpDataStorage : public IRemoteDataStorage
{
public:
    CHttpDataStorage(QObject * a_pParent = 0);

    ~CHttpDataStorage();

    // IRemoteDataStorage interface
public:
    virtual void push(QVector<IData> const & a_crData);
    virtual void pull(QVector<IData> & a_rData);
    virtual IRemoteDataStorage::ERemoteDataStorageSts status() const;

private:
    IRemoteDataStorage::ERemoteDataStorageSts m_eDataStorageSts;
    CHttpBrowser m_oHttpBrowser;
};

#endif // CSQLDATASTORAGE_H
