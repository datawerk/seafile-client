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

void AvatarManager::getAvatarUrl(const QString &avatar_url)
{
    QUrl url(server_url_);
    url.setPath(avatar_url);
    QString url_str = url.toString();
    if (!req_list_.contains(url_str)) {
        dld_avatar_req_ = new DownloadAvatarRequest(url_str);
        connect(dld_avatar_req_, SIGNAL(success(const QByteArray&)),
                this, SLOT(downloadAvatar(const QByteArray&)));
        connect(dld_avatar_req_, SIGNAL(failed(int)), this, SLOT(downloadAvatarFailed(int)));
        dld_avatar_req_->send();
        req_list_.push_back(url_str);
    }
}

void AvatarManager::getAvatarUrlFailed(int code)
{
    printf("Get avatar url failed,the error code is %d\n", code);
}

void AvatarManager::downloadAvatar(const QByteArray& avatar_byte)
{
    QString avatar_name = hash(user_ + server_url_);
    saveAvatar(avatar_byte, avatar_name);
}

void AvatarManager::downloadAvatarFailed(int code)
{
    printf("Download avatar failed,the error code is %d\n", code);
}

void AvatarManager::requestAvatar(const Account &account, const QString &user)
{
    server_url_ = account.serverUrl.toString();
    user_ = user;
    get_avatar_req_ = new GetAvatarRequest(account, user);
    connect(get_avatar_req_, SIGNAL(success(const QString&)),
            this, SLOT(getAvatarUrl(const QString&)));
    connect(get_avatar_req_, SIGNAL(failed(int)), this, SLOT(getAvatarUrlFailed(int)));
    get_avatar_req_->send();
}

void AvatarManager::saveAvatar(const QByteArray& avatar, QString& avatar_name)
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

