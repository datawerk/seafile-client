#ifndef AVATARMGR_H
#define AVATARMGR_H

#include<QObject>
#include<QCryptographicHash>
#include"api/seafile-events.h"
#include"account.h"
#include <QSslError>

class QNetworkReply;
class GetAvatarRequest;
class QNetworkAccessManager;

struct sqlite3;
struct sqlite3_stmt;


class AvatarManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarManager(QObject *parent = 0);
    
public:
    QIcon getAvatar(const Account& account, QString& user);
    int start();

signals:
    void getAvatarSuccess(const QString& avatar_user);

private slots:
    void getAvatarUrl(const QString& avatar_url);
    void getAvatarUrlFailed(int);
    void replyFinished(QNetworkReply *reply);
    void onSslErrors(QList<QSslError>);
private:
    void requestAvatar(const Account& account, const QString& user);
    void saveAvatar(QByteArray avatar, QString& avatar_name);
    QString hash(const QString& str, QCryptographicHash::Algorithm algorithm=QCryptographicHash::Sha1);
    QString avatarFormat(const QString& avatar_url);
private:
    QString server_url_;
    QString user_;
    QString avatars_dir_;
    QString avatar_format_;
    GetAvatarRequest* get_avatar_req_;
    QNetworkAccessManager *network_manager_;
    QNetworkReply* reply_;

    struct sqlite3 *db;
};

#endif // AVATARMGR_H
