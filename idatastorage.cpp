#include "idatastorage.h"

#include <QDebug>

IDataStorage::IDataStorage()
{

}

IDataStorage::~IDataStorage()
{

}

bool IDataStorage::commitData(int a_iDataTypeId, QString a_strData)
{
    Q_UNUSED(a_iDataTypeId)
    Q_UNUSED(a_strData)

    qWarning() << "type not supported: QString";

    return false;
}

bool IDataStorage::commitData(int a_iDataTypeId, int a_iData)
{
    Q_UNUSED(a_iDataTypeId)
    Q_UNUSED(a_iData)

    qWarning() << "type not supported: int";

    return false;
}

bool IDataStorage::commitData(int a_iDataTypeId, double a_dData)
{
    Q_UNUSED(a_iDataTypeId)
    Q_UNUSED(a_dData)

    qWarning() << "type not supported: double";

    return false;
}

