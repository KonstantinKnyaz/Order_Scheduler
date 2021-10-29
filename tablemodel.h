#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QList>

#include "datamodel.h"

class tableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    tableModel(QObject *parent = Q_NULLPTR);
    ~tableModel();

    int rowCount(const QModelIndex &parent) const;
    void setRowCount(const int &row);
    int columnCount(const QModelIndex &parent) const;
    void setColumnCount(const int &column);
    QVariant data(const QModelIndex &idx, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void add(dataModel value);
    void update(const QModelIndex &index, dataModel value);
    void remove(const QModelIndex &index);
    QList<dataModel> values;
    
private:
    int m_row = 0;
    int m_column = 0;
};


#endif // TABLEMODEL_H
