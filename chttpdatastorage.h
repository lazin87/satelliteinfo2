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
    virtual void push(std::unique_ptr<IData> const & a_crData);
    virtual void pull(QVector<IData> & a_rData);
    virtual ERemoteDataStorageSts status() const;

private:
    ERemoteDataStorageSts m_eDataStorageSts;
    CHttpBrowser m_oHttpBrowser;

    const QString strTARGET_URL = "http://lazinski.pl/QtTest/includes/addRow.php";
};

#endif // CSQLDATASTORAGE_H
