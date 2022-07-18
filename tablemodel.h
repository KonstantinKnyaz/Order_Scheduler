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

    void setRowCount(const int &row);
    void setColumnCount(const int &column);
    void add(dataModel value);
    void update(const QModelIndex &index, dataModel value);
    void remove(const QModelIndex &index);
    void setDefaultRowColor(const bool &def);
    QList<dataModel> values = Q_NULLPTR;
    
private:
    int m_row = 0;
    int m_column = 0;
    bool defCol = false;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &idx, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};


#endif // TABLEMODEL_H
