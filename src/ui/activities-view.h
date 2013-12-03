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

private slots:
    void showContextMenu(const QPoint& pos);
    void actionTriggered(QAction*);
private:
    void createContextMenu();
    void openLocalFile();
    void viewFileOneWeb();
private:
    QMenu* context_menu_;
    QAction* open_local_folder_action_;
    QAction* view_on_web_action_;

};

#endif // ACTIVITIESVIEW_H
