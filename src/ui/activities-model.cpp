#include "activities-model.h"
#include "utils/utils.h"
#include <QIcon>
#include "avatar-mgr.h"
#include "activities-delegate.h"

ActivitiesModel::ActivitiesModel(QObject *parent) :
    QStandardItemModel(parent)
{
    avatar_manager_ = new AvatarManager;
    connect(avatar_manager_, SIGNAL(getAvatarSuccess(const QString&)), this, SLOT(avatarReady(const QString&)));
}

void ActivitiesModel::setEvents(const SeafileEvents &events)
{
    /*pop loading... item*/
    if (!events_.events_list.empty()) {
        events_.events_list.pop_back();
    }

    SeafileEvent event;
    QStandardItem *item;
    QIcon icon;
    for (size_t i = 0; i < events.events_list.size(); ++i) {
        event = events.events_list.at(i);
        events_.events_list.push_back(events.events_list.at(i));
        if (!event.is_last) {
            item = new QStandardItem;
            icon = avatar_manager_->getAvatar(current_account_, event.author);
            item->setData(icon, ActivitiesDelegate::ICON_ROLE);
            item->setData(event.descrip(), ActivitiesDelegate::HEADER_TEXT_ROLE);
            item->setData(translateCommitTime(event.mtime), ActivitiesDelegate::SUB_TEXT_ROLE);
            item->setEditable(false);
            appendRow(item);
        }
    }

    events_.more = events.more;
    events_.more_offset = events.more_offset;

    reset();
}

void ActivitiesModel::setCurrentAccount(Account &account)
{
    current_account_ = account;
}

void ActivitiesModel::clear()
{
    events_.events_list.clear();
}

void ActivitiesModel::avatarReady(const QString& avatar_user)
{
    avatar_user_ = avatar_user;
    SeafileEvent event;
    for (size_t i = 0; i < events_.events_list.size(); ++i) {
        event = events_.events_list.at(i);
        if (event.author == avatar_user) {
            //QModelIndex index = createIndex(int(i), 0);
            QIcon icon = avatar_manager_->getAvatar(current_account_, event.author);
            item(i)->setData(icon, ActivitiesDelegate::ICON_ROLE);
            reset();
        }

    }
    printf("Model get avatar user is %s\n", toCStr(avatar_user_));
}

//QVariant ActivitiesModel::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid()) {
//        return QVariant();
//    }

//    SeafileEvent event = events_.events_list.at(index.row());
//    if (!event.is_last && role == ActivitiesDelegate::ICON_ROLE) {
//        return avatar_manager_->getAvatar(current_account_, event.author);
//    }

//    if (role == ActivitiesDelegate::HEADER_TEXT_ROLE) {
//        if (event.is_last) {

//            if (events_.more) {
//                return event.descrip();
//            } else {
//                return QObject::tr("No more activities.");
//            }

//        }
//        return event.descrip();
//    }

//    if (role == ActivitiesDelegate::SUB_TEXT_ROLE) {
//        return translateCommitTime(event.mtime);
//    }

//    return QVariant();

//}
