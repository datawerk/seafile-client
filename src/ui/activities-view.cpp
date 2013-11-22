#include "activities-view.h"


ActivitiesView::ActivitiesView(QListView *parent) :
    QListView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setViewMode(QListView::IconMode);
    //setFlow(QListView::LeftToRight);
    //setWrapping(true);
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    setIconSize(QSize(50, 50));
    //setGridSize(QSize(width(), 60));
}

ActivitiesView::~ActivitiesView()
{

}
