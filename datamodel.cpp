#include "datamodel.h"

dataModel::dataModel(QString name, QString phone, QString order, QString desc, QString date) :
    _name(name),
    _phone(phone),
    _order(order),
    _desc(desc),
    _date(date)
{

}

void dataModel::setName(const QString &value)
{
    _name = value;
}

QString dataModel::getName() const
{
    return _name;
}

void dataModel::setPhone(const QString &value)
{
    _phone = value;
}

QString dataModel::getPhone() const
{
    return _phone;
}

void dataModel::setOrder(const QString &value)
{
    _order = value;
}

QString dataModel::getOrder() const
{
    return _order;
}

void dataModel::setDesc(const QString &value)
{
    _desc = value;
}

QString dataModel::getDesc() const
{
    return _desc;
}

void dataModel::setDate(const QString &value)
{
   _date = value;
}

QString dataModel::getDate() const
{
    return _date;
}
