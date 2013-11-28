#include "activities-delegate.h"

ActivitiesDelegate::ActivitiesDelegate()
{
}

void ActivitiesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

QSize ActivitiesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0, 0);
}
