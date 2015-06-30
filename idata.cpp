#include "idata.h"

#include <QDebug>

IData::IData()
{

}

IData::~IData()
{

}

bool IData::set(int a_iColumn, double a_dVal)
{
    Q_UNUSED(a_iColumn)
    Q_UNUSED(a_dVal)

    qWarning() << "IData::set() should be overriden!";
    return false;
}

bool IData::set(QVector<double>::iterator a_itrBegin, QVector<double>::iterator a_itrEnd)
{
    Q_UNUSED(a_itrBegin)
    Q_UNUSED(a_itrEnd)

    qWarning() << "IData::set() should be overriden!";
    return false;
}

double IData::get(int a_iColumn) const
{
    Q_UNUSED(a_iColumn)

    qWarning() << "IData::get() should be overriden!";
    return -1.0;
}

