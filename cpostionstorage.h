#ifndef CPOSTIONSTORAGE_H
#define CPOSTIONSTORAGE_H

#include <QObject>
#include "idatastorage.h"

class CDataStorage : public QObject, public IDataStorage
{
    Q_OBJECT
public:
    explicit CDataStorage(QObject *parent = 0);
    ~CDataStorage();

    bool commitData(CData const & a_rData);
    bool pushData();
    bool pullData(CData & a_rOutData);
    int status();

signals:

public slots:

private:

};

#endif // CPOSTIONSTORAGE_H
