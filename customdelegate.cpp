#include "customdelegate.h"

#include <QPainter>
#include <QAbstractItemView>
#include <QColor>

CustomDelegate::CustomDelegate(QObject* parent) : BaseClass(parent)
{
}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->fillRect(option.rect, Qt::red);

    BaseClass::paint(painter, option, index);
}

CustomDelegateWhite::CustomDelegateWhite(QObject* parent) : BaseClass(parent)
{
}

void CustomDelegateWhite::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (QAbstractItemView* tableView = qobject_cast<QAbstractItemView*>(this->parent()))
    {
            painter->fillRect(option.rect, Qt::white);
    }

    BaseClass::paint(painter, option, index);
}
