#ifndef AVATARMGR_H
#define AVATARMGR_H

#include<QObject>
#include<QCryptographicHash>
#include"api/seafile-events.h"
#include"account.h"
#include <QList>

class QNetworkReply;
class GetAvatarRequest;
class DownloadAvatarRequest;
class QNetworkAccessManager;


class AvatarManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarManager(QObject *parent = 0);
    
public:
    QIcon getAvatar(const Account& account, QString& user);

signals:
    void getAvatarSuccess(const QString& avatar_user);

private slots:
    void getAvatarUrl(const QString& avatar_url);
    void getAvatarUrlFailed(int);
    void downloadAvatar(const QByteArray& avatar_byte);
    void downloadAvatarFailed(int);
private:
    void requestAvatar(const Account& account, const QString& user);
    void saveAvatar(const QByteArray& avatar, QString& avatar_name);
    QString hash(const QString& str, QCryptographicHash::Algorithm algorithm=QCryptographicHash::Sha1);
private:
    QString server_url_;
    QString user_;
    QString avatars_dir_;
    QString avatar_format_;
    GetAvatarRequest* get_avatar_req_;
    DownloadAvatarRequest* dld_avatar_req_;
    QList<QString> req_list_;
};

#endif // AVATARMGR_H
