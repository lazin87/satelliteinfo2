#ifndef IDATA_H
#define IDATA_H

#include <QVector>

class IData
{
public:
    IData();
    virtual ~IData();

    virtual bool set(int a_iColumn, double a_dVal);
    virtual bool set(QVector<double>::iterator a_itrBegin, QVector<double>::iterator a_itrEnd);

    // return true if output value is valid, otherwise false
    virtual double get(int a_iColumn) const; // przerobic getowanie na iRow z double

    virtual QString toString() = 0;
    virtual QString columnName(int a_iRow) const = 0;
    virtual int count() const = 0;

};

#endif // IDATA_H
