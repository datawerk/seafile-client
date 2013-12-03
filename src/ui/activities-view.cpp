#include "activities-view.h"
#include <QAction>
#include <QMenu>
#include "utils/utils.h"

ActivitiesView::ActivitiesView(QListView *parent) :
    QListView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::CustomContextMenu);
    createContextMenu();
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
    connect(context_menu_, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

}

ActivitiesView::~ActivitiesView()
{

}

void ActivitiesView::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = mapToGlobal(pos);
    context_menu_->popup(globalPos);
}

void ActivitiesView::actionTriggered(QAction *action)
{
    if (action->iconText() == "Open file") {
        openLocalFile();
    } else if (action->iconText() == "View on cloud") {
        viewFileOneWeb();
    }
}

void ActivitiesView::createContextMenu()
{
    context_menu_ = new QMenu(this);

    open_local_folder_action_ = new QAction(tr("&Open file"), this);
    open_local_folder_action_->setIcon(QIcon(":/images/folder-open.png"));
    open_local_folder_action_->setStatusTip(tr("open local file"));
    open_local_folder_action_->setIconVisibleInMenu(true);

    view_on_web_action_  = new QAction(tr("&View on cloud"), this);
    view_on_web_action_->setIcon(QIcon(":/images/cloud.png"));
    view_on_web_action_->setStatusTip(tr("view this library on seahub"));
    view_on_web_action_->setIconVisibleInMenu(true);

    context_menu_->addAction(open_local_folder_action_);
    context_menu_->addAction(view_on_web_action_);
}

void ActivitiesView::openLocalFile()
{
    printf("The OPEN action triggered.\n");
}

void ActivitiesView::viewFileOneWeb()
{
    printf("The VIEW action triggered.\n");
}
