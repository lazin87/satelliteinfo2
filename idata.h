#ifndef IDATA_H
#define IDATA_H

#include <QVector>

class IData
{
public:
    IData();
    virtual ~IData();

    virtual bool set(QVector<double>::iterator a_iterBegin, QVector<double>::iterator a_iterEnd);
    virtual QString toString() = 0;
    virtual QString columnName(int a_iRow) const = 0;
    // return true if output value is valid, otherwise false
    virtual bool get(int a_iRow, void * a_outVal) const = 0;
};

#endif // IDATA_H
