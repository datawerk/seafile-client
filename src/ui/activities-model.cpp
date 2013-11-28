#include "activities-model.h"
#include "utils/utils.h"
#include <QIcon>
#include "avatar-mgr.h"

ActivitiesModel::ActivitiesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    avatar_manager_ = new AvatarManager;
    connect(avatar_manager_, SIGNAL(getAvatarSuccess(const QString&)), this, SLOT(avatarReady(const QString&)));
}

void ActivitiesModel::setEvents(const SeafileEvents &events)
{
    //pop loading... item
    if (!events_.events_list.empty()) {
        events_.events_list.pop_back();
    }

    SeafileEvent event;
    for (size_t i = 0; i < events.events_list.size(); ++i) {
        event = events.events_list.at(i);
        events_.events_list.push_back(events.events_list.at(i));
    }
    events_.more = events.more;
    events_.more_offset = events.more_offset;


    reset();
}

int ActivitiesModel::rowCount(const QModelIndex &parent) const
{
    return events_.events_list.size();
}

void ActivitiesModel::insertData(const QVariant &variant)
{
    //insertRow();
}

void ActivitiesModel::setCurrentAccount(Account &account)
{
    current_account_ = account;
}

void ActivitiesModel::avatarReady(const QString& avatar_user)
{
    avatar_user_ = avatar_user;
    SeafileEvent event;
    for (size_t i = 0; i < events_.events_list.size(); ++i) {
        event = events_.events_list.at(i);
        if (event.author == avatar_user) {
            QModelIndex index = createIndex(int(i), 0);
            emit dataChanged(index, index);
        }

    }
    printf("Model get avatar user is %s\n", toCStr(avatar_user_));
}

QVariant ActivitiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    SeafileEvent event = events_.events_list.at(index.row());
    if (!event.is_last && role == Qt::DecorationRole) {
        return avatar_manager_->getAvatar(current_account_, event.author);
    }

    if (role == Qt::DisplayRole) {
        if (event.is_last) {

            if (events_.more) {
                return event.descrip();
            } else {
                return QObject::tr("No more activities.");
            }

        }
        return event.nick_name + " " + event.descrip();


    }

    return QVariant();

}
