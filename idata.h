#ifndef IDATA_H
#define IDATA_H


class IData
{
public:
    IData();
    ~IData();

    virtual bool set() = 0;
    virtual QString toString() = 0;
    virtual QString columnName(int a_iRow) const = 0;
};

#endif // IDATA_H
