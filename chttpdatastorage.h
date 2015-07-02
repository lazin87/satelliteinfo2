#ifndef CSQLDATASTORAGE_H
#define CSQLDATASTORAGE_H


#include "iremotedatastorage.h"
#include "chttpbrowser.h"


class CHttpDataStorage : public IRemoteDataStorage
{
public:
    const QString strTARGET_URL = "http://lazinski.pl/QtTest/includes/addRow.php";

    CHttpDataStorage(CHttpBrowser & a_rHttpBrowser);

    ~CHttpDataStorage();

    // IRemoteDataStorage interface
public:
    virtual void push(IData const & a_crData);
    virtual void pull(QVector<IData> & a_rData);
    virtual ERemoteDataStorageSts status() const;

private:
    ERemoteDataStorageSts m_eDataStorageSts;
    CHttpBrowser * m_pHttpBrowser;
};

#endif // CSQLDATASTORAGE_H
