#ifndef ACTIVITIESDELEGATE_H
#define ACTIVITIESDELEGATE_H

#include <QStyledItemDelegate>

class ActivitiesDelegate : public QStyledItemDelegate
{

public:
    explicit ActivitiesDelegate();
    
public:
    enum DataRole {HEADER_TEXT_ROLE = 1,
                   SUB_TEXT_ROLE,
                   ICON_ROLE};
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ACTIVITIESDELEGATE_H
