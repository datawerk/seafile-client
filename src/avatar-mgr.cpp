#include "avatar-mgr.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QIcon>
#include <QFile>
#include "api/requests.h"
#include "utils/utils.h"
#include "configurator.h"
#include "seafile-applet.h"
#include "sqlite3.h"

AvatarManager::AvatarManager(QObject *parent) :
    QObject(parent)
{
    avatars_dir_ = seafApplet->configurator()->seafileDir();
    avatars_dir_.append("/avatars/");
    QDir avatars_dir(avatars_dir_);
    if (!avatars_dir.exists()) {
        avatars_dir.cdUp();
        avatars_dir.mkpath("avatars");
    }
}

QIcon AvatarManager::getAvatar(const Account& account, QString& user)
{
    QString avatar_name = hash(user + account.serverUrl.toString());
    const QString avatar_path = avatars_dir_ + avatar_name;
    QFile avatar_file(avatar_path);
    if (avatar_file.exists()) {
        return QIcon(avatar_path);
    } else {
        requestAvatar(account, user);
        return QIcon(":/images/account.png");
    }

}

int AvatarManager::start()
{
    const char *errmsg;
    const char *sql;

    QString db_path = QDir(seafApplet->configurator()->seafileDir()).filePath("avatars.db");
    if (sqlite3_open (toCStr(db_path), &db)) {
        errmsg = sqlite3_errmsg (db);
        qDebug("failed to open avatars database %s: %s",
               toCStr(db_path), errmsg ? errmsg : "no error given");

        seafApplet->errorAndExit(tr("failed to open avatars databse"));
        return -1;
    }

    sql = "CREATE TABLE IF NOT EXISTS Avatars (url VARCHAR(24), "
        "username VARCHAR(15), token VARCHAR(40), lastVisited INTEGER, "
        "PRIMARY KEY(url, username))";
    sqlite_query_exec (db, sql);

    return 0;
}

void AvatarManager::getAvatarUrl(const QString &avatar_url)
{

    QNetworkRequest request;
    QUrl url(server_url_);
    url.setPath(avatar_url);
    request.setUrl(url);
    network_manager_ = new QNetworkAccessManager(this);
    //avatar_format_ = avatarFormat(avatar_url);
    reply_ =  network_manager_->get(request);
    connect(reply_, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
    connect(network_manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void AvatarManager::getAvatarUrlFailed(int code)
{
    printf("Get avatar failed,the error code is %d\n", code);
}

void AvatarManager::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        printf("Error happened.Error code is %d\n", reply->error());
    } else {
        QByteArray avatar = reply->readAll();
        QString avatar_name = hash(user_ + server_url_);
        saveAvatar(avatar, avatar_name);
    }
}

void AvatarManager::onSslErrors(QList<QSslError>)
{
    reply_->ignoreSslErrors();
}

void AvatarManager::requestAvatar(const Account &account, const QString &user)
{
    get_avatar_req_ = new GetAvatarRequest(account, user);
    server_url_ = account.serverUrl.toString();
    user_ = user;
    connect(get_avatar_req_, SIGNAL(success(const QString&)),
            this, SLOT(getAvatarUrl(const QString&)));
    connect(get_avatar_req_, SIGNAL(failed(int)), this, SLOT(getAvatarUrlFailed(int)));
    get_avatar_req_->send();
}

void AvatarManager::saveAvatar(QByteArray avatar, QString& avatar_name)
{
    QFile avatar_file(avatars_dir_ + avatar_name);
    avatar_file.open(QIODevice::WriteOnly);
    qint64 value = avatar_file.write(avatar);
    if (value != -1) {
        emit getAvatarSuccess(user_);
    }
    avatar_file.close();
}

QString AvatarManager::hash(const QString& str, QCryptographicHash::Algorithm algorithm)
{
    QByteArray data = str.toUtf8();
    QByteArray result = QCryptographicHash::hash(data, algorithm);
    result = result.toHex();

    return QString(result);
}

QString AvatarManager::avatarFormat(const QString &avatar_url)
{
    int length = avatar_url.length();
    int index;

    for (size_t i = length-1; i > 0; --i) {
        if (avatar_url.at(i) == QChar('.')) {
            index = i;
            break;
        }
    }

    return avatar_url.right(length-index);
}
