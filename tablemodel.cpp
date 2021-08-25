#include "tablemodel.h"

#include <QLocale>

tableModel::tableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

tableModel::~tableModel()
{
    deleteLater();
}

int tableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return values.count();
}

int tableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

QVariant tableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    switch (role) {
    case Qt::DisplayRole:
    {
        switch (index.column()) {
            case 0: {
                value = this->values.at(index.row()).getName();
                break;
            }
            case 1: {
                value = this->values.at(index.row()).getPhone();
                break;
            }
            case 2: {
                value = this->values.at(index.row()).getOrder();
                break;
            }
            case 3: {
                value = this->values.at(index.row()).getDesc();
                break;
            }
            case 4: {
                value = this->values.at(index.row()).getDate();
                break;
            }
        }
    }
        break;
    default:
        break;
    }
    return value;
}

QVariant tableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return QString("ФИО");
            case 1:
                return QString("Телефон");
            case 2:
                return QString("Заказ");
            case 3:
                return QString("Описание");
            case 4:
                return QString("Дата");
        }
    }
    return QVariant();
}

void tableModel::add(dataModel value)
{
    int newRow = this->values.count();

    this->beginInsertRows(QModelIndex(), newRow, newRow);
        values.append(value);
    endInsertRows();
}

void tableModel::update(const QModelIndex &index, dataModel value)
{
    values[index.row()] = value;
    emit this->dataChanged(index, index);
}

void tableModel::remove(const QModelIndex &index)
{
    this->beginRemoveRows(QModelIndex(), index.row(), index.row());
        values.removeAt(index.row());
    this->endRemoveRows();
}
