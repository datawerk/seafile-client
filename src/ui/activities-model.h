#ifndef ACTIVITIESMODEL_H
#define ACTIVITIESMODEL_H

#include <QAbstractListModel>
#include <QStandardItemModel>
#include "api/seafile-events.h"
#include "account.h"

class SeafileEvents;
class AvatarManager;

class ActivitiesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ActivitiesModel(QObject *parent = 0);
    void setEvents(const SeafileEvents& events);
    inline SeafileEvents events(){return events_;}
    //int rowCount(const QModelIndex &parent) const;
    void setCurrentAccount(Account& account);
    void clear();
signals:
    
public slots:
    void avatarReady(const QString&);
protected:

    //QVariant data(const QModelIndex &index, int role) const;

private:
    Account current_account_;
    SeafileEvents events_;
    AvatarManager* avatar_manager_;
    QString avatar_user_;
};

#endif // ACTIVITIESMODEL_H
