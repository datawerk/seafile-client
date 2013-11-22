#include "activities-model.h"
#include "utils/utils.h"
#include <QIcon>

ActivitiesModel::ActivitiesModel(QObject *parent) :
    QStandardItemModel(parent)
{

}

void ActivitiesModel::setEvents(const SeafileEvents &events)
{
    for (size_t i = 0; i < events.events_list.size(); ++i) {
        events_.events_list.push_back(events.events_list.at(i));
    }
    events_.more = events.more;
    events_.more_offset = events.more_offset;

//    QStandardItem* item;
//    SeafileEvent event;
//    for (size_t i = 0; i < events.events_list.size(); ++i) {
//        event = events.events_list.at(i);
//        item = new QStandardItem(QIcon(":/images/account.png"), event.nick_name +" "+ event.descrip());
//        appendRow(item);
//    }

    reset();
}

void ActivitiesModel::clear()
{
    QStandardItemModel::clear();
    events_.events_list.clear();
}

int ActivitiesModel::rowCount(const QModelIndex &parent) const
{
    return events_.events_list.size();
}

QVariant ActivitiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (role == Qt::DecorationRole) {
        return QIcon(":/images/account.png");
    }

    if (role == Qt::DisplayRole) {
        SeafileEvent event = events_.events_list.at(index.row());
        return event.nick_name + " " + event.descrip();
    }

    return QVariant();

}
