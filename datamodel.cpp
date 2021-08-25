#include "datamodel.h"

dataModel::dataModel(QString name, QString phone, QString order, QString desc, QString date)
{
    this->name = name;
    this->phone = phone;
    this->order = order;
    this->desc = desc;
    this->date = date;

}

void dataModel::setName(const QString &value)
{
    name = value;
}

QString dataModel::getName() const
{
    return name;
}

void dataModel::setPhone(const QString &value)
{
    phone = value;
}

QString dataModel::getPhone() const
{
    return phone;
}

void dataModel::setOrder(const QString &value)
{
    order = value;
}

QString dataModel::getOrder() const
{
    return order;
}

void dataModel::setDesc(const QString &value)
{
    desc = value;
}

QString dataModel::getDesc() const
{
    return desc;
}

void dataModel::setDate(const QString &value)
{
 date = value;
}

QString dataModel::getDate() const
{
    return date;
}
