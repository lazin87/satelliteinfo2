#ifndef CDATA_H
#define CDATA_H

#include <QString>
#include <QVector>
#include <QPair>

class CData
{
public:
    CData();
    ~CData();

    bool add(QString a_strName, QString a_strValue);
    bool remove(QString a_strName);
    int count() const;
    void clear();

private:
    QVector<QPair<QString, QString> > m_aParams;
};

#endif // CDATA_H
