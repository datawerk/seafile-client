#ifndef ACTIVITIESVIEW_H
#define ACTIVITIESVIEW_H

#include <QListView>

class ActivitiesView : public QListView
{
    Q_OBJECT
    
public:
    explicit ActivitiesView(QListView *parent = 0);
    ~ActivitiesView();
    
public:

};

#endif // ACTIVITIESVIEW_H
