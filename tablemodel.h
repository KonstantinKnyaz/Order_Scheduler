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
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void add(dataModel value);
    void update(const QModelIndex &index, dataModel value);
    void remove(const QModelIndex &index);
    
private:
    QList<dataModel> values;
};


#endif // TABLEMODEL_H
