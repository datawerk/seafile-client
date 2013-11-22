#ifndef ACTIVITIESMODEL_H
#define ACTIVITIESMODEL_H

#include <QAbstractListModel>
#include "api/seafile-events.h"

class SeafileEvents;

class ActivitiesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivitiesModel(QObject *parent = 0);
    void setEvents(const SeafileEvents& events);
    inline SeafileEvents events(){return events_;}
    void clear();
signals:
    
public slots:

protected:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:

    SeafileEvents events_;
};

#endif // ACTIVITIESMODEL_H
