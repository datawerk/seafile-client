#ifndef AVATARMGR_H
#define AVATARMGR_H

#include <QObject>

class AvatarManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarManager(QObject *parent = 0);
    
public:
    void getAvatar(const QString& avatar_url);

signals:
    
public slots:
    
};

#endif // AVATARMGR_H
