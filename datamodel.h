#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QString>

class dataModel
{
public:
    dataModel(QString name, QString phone, QString order, QString desc, QString date);

    void setName(const QString &value);
    QString getName() const;

    void setPhone(const QString &value);
    QString getPhone() const;

    void setOrder(const QString &value);
    QString getOrder() const;

    void setDesc(const QString &value);
    QString getDesc() const;

    void setDate(const QString &value);
    QString getDate() const;

private:
    QString _name;
    QString _phone;
    QString _order;
    QString _desc;
    QString _date;
};

#endif // DATAMODEL_H
