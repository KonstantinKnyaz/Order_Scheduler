#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QStyledItemDelegate>

class CustomDelegate : public QStyledItemDelegate
{
    using BaseClass = QStyledItemDelegate;

public:
    CustomDelegate(QObject* parent = Q_NULLPTR);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class CustomDelegateWhite : public QStyledItemDelegate
{
    using BaseClass = QStyledItemDelegate;

public:
    CustomDelegateWhite(QObject* parent = Q_NULLPTR);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CUSTOMDELEGATE_H
