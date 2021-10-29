#include "tablemodel.h"

#include <QLocale>
#include <QBrush>
#include <QDate>

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

void tableModel::setRowCount(const int &row)
{
    m_row = row;
}

int tableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_column;
}

void tableModel::setColumnCount(const int &column)
{
    m_column = column;
}

QVariant tableModel::data(const QModelIndex &idx, int role) const
{
    QVariant value;
    if(!defCol) {
        if(role == Qt::BackgroundColorRole) {
            QDate date = QDate::currentDate();
            if(!date.isValid()) return value;
            QDate tableData = index(idx.row(), 4, idx).data().toDate();
            quint64 days =  date.daysTo(tableData);
            if(days < 1)
                return QVariant(QColor(255,0,0,255));
            else if (days < 3)
                return QVariant(QColor(255,217,0,255));
            else
                return QVariant(QColor(39,39,39,255));
        }

        if(role == Qt::ForegroundRole) {
            QDate date = QDate::currentDate();
            if(!date.isValid()) return value;
            QDate tableData = index(idx.row(), 4, idx).data().toDate();
            quint64 days =  date.daysTo(tableData);
            if(days < 1)
                return QVariant(QColor(255,255,255,255));
            else if(days < 3)
                return QVariant(QColor(47,47,47,255));
            else
                return QVariant(QColor(255,255,255,255));
        }
    }

    switch (role) {
    case Qt::DisplayRole:
    {
        switch (idx.column()) {
            case 0: {
                value = this->values.at(idx.row()).getName();
                break;
            }
            case 1: {
                value = this->values.at(idx.row()).getPhone();
                break;
            }
            case 2: {
                value = this->values.at(idx.row()).getOrder();
                break;
            }
            case 3: {
                value = this->values.at(idx.row()).getDesc();
                break;
            }
            case 4: {
                value = this->values.at(idx.row()).getDate();
                break;
            }
        }
    }
        break;
    default:
        return value;
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

    if(role == Qt::BackgroundColorRole && orientation == Qt::Horizontal) {
        return QVariant(QColor(39, 39, 39, 255));
    }

    if(role == Qt::ForegroundRole && orientation == Qt::Horizontal) {
        return QVariant(QColor(255, 255, 255, 255));
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

void tableModel::setDefaultRowColor(const bool &def)
{
    defCol = def;
}
