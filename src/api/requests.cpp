#include <jansson.h>

#include <QtNetwork>
#include <QScopedPointer>

#include "account.h"

#include "utils/utils.h"
#include "requests.h"
#include "server-repo.h"

namespace {

const char *kApiLoginUrl = "/api2/auth-token/";
const char *kListReposUrl = "/api2/repos/";
const char *kCreateRepoUrl = "/api2/repos/";
const char *kMessagesCountUrl = "/api2/msgs_count/";
const char *kEventsUrl = "/api2/events/";
const char *kAvatarUrl = "/api2/avatar/";

} // namespace


/**
 * LoginRequest
 */
LoginRequest::LoginRequest(const QUrl& serverAddr,
                           const QString& username,
                           const QString& password)

    : SeafileApiRequest (QUrl(serverAddr.toString() + kApiLoginUrl),
                         SeafileApiRequest::METHOD_POST)
{
    setParam("username", username);
    setParam("password", password);
}

void LoginRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);

    const char *token = json_string_value(json_object_get(json.data(), "token"));
    if (token == NULL) {
        qDebug("failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    qDebug("login successful, token is %s\n", token);

    emit success(token);
}


/**
 * ListReposRequest
 */
ListReposRequest::ListReposRequest(const Account& account)
    : SeafileApiRequest (QUrl(account.serverUrl.toString() + kListReposUrl),
                         SeafileApiRequest::METHOD_GET, account.token)
{
}

void ListReposRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("ListReposRequest:failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);

    std::vector<ServerRepo> repos = ServerRepo::listFromJSON(json.data(), &error);
    emit success(repos);
}


/**
 * DownloadRepoRequest
 */
DownloadRepoRequest::DownloadRepoRequest(const Account& account, const ServerRepo& repo)
    : SeafileApiRequest(QUrl(account.serverUrl.toString() + "/api2/repos/" + repo.id + "/download-info/"),
                        SeafileApiRequest::METHOD_GET, account.token),
      repo_(repo)
{
}

RepoDownloadInfo RepoDownloadInfo::fromDict(QMap<QString, QVariant>& dict)
{
    RepoDownloadInfo info;
    info.relay_id = dict["relay_id"].toString();
    info.relay_addr = dict["relay_addr"].toString();
    info.relay_port = dict["relay_port"].toString();
    info.email = dict["email"].toString();
    info.token = dict["token"].toString();
    info.repo_id = dict["repo_id"].toString();
    info.repo_name = dict["repo_name"].toString();
    info.encrypted = dict["encrypted"].toInt();
    info.magic = dict["magic"].toString();
    info.random_key = dict["random_key"].toString();
    info.enc_version = dict.value("enc_version", 1).toInt();

    return info;
}

void DownloadRepoRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);
    QMap<QString, QVariant> dict = mapFromJSON(json.data(), &error);

    RepoDownloadInfo info = RepoDownloadInfo::fromDict(dict);

    info.relay_addr = url().host();

    emit success(info);
}

/**
 * CreateRepoRequest
 */
CreateRepoRequest::CreateRepoRequest(const Account& account, QString &name, QString &desc, QString &passwd)
    : SeafileApiRequest (QUrl(account.serverUrl.toString() + kCreateRepoUrl),
                         SeafileApiRequest::METHOD_POST, account.token)
{
    this->setParam(QString("name"), name);
    this->setParam(QString("desc"), desc);
    if (!passwd.isNull()) {
        qDebug("Encrypted repo");
        this->setParam(QString("passwd"), passwd);
    }
}

void CreateRepoRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);
    QMap<QString, QVariant> dict = mapFromJSON(json.data(), &error);
    RepoDownloadInfo info = RepoDownloadInfo::fromDict(dict);

    info.relay_addr = url().host();
    emit success(info);
}

/**
 * GetSeahubMessagesRequest
 */
GetSeahubMessagesRequest::GetSeahubMessagesRequest(const Account& account)
    : SeafileApiRequest (QUrl(account.serverUrl.toString() + kMessagesCountUrl),
                         SeafileApiRequest::METHOD_GET, account.token)
{
}

void GetSeahubMessagesRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("GetSeahubMessagesRequest: failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }

    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);

    QMap<QString, QVariant> ret = mapFromJSON(root, &error);

    if (!ret.contains("personal_messages") || !ret.contains("group_messages")) {
        emit failed(0);
        return;
    }

    int group_messages = ret.value("group_messages").toInt();
    int personal_messages = ret.value("personal_messages").toInt();
    emit success(group_messages, personal_messages);
}

/*
 * GetEventsRequest
 */
GetEventsRequest::GetEventsRequest(const Account& account)
    : SeafileApiRequest (QUrl(account.serverUrl.toString() + kEventsUrl),
                         SeafileApiRequest::METHOD_GET, account.token)
{

}

void GetEventsRequest::setEventsOffset(const QString &name, const QString &offset)
{
    setParam(name, offset);
}

void GetEventsRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("GetEventsRequest:failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }
    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);

    SeafileEvents events = SeafileEvents::fromJSON(json.data(), &error);
    emit success(events);
}
/*
 * GetAvatarRequest
 */
GetAvatarRequest::GetAvatarRequest(const Account& account, const QString& avatar_user)
    : SeafileApiRequest (QUrl(account.serverUrl.toString() + kAvatarUrl),
                         SeafileApiRequest::METHOD_GET, account.token)
{
    setParam("user", avatar_user);
    setParam("size", "32");
}

void GetAvatarRequest::requestSuccess(QNetworkReply& reply)
{
    json_error_t error;
    json_t *root = parseJSON(reply, &error);
    if (!root) {
        qDebug("GetAvatarRequest:failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }
    QScopedPointer<json_t, JsonPointerCustomDeleter> json(root);

    const char *avatar_url = json_string_value(json_object_get(json.data(), "url"));
    if (avatar_url == NULL) {
        qDebug("failed to parse json:%s\n", error.text);
        emit failed(0);
        return;
    }
    QByteArray text = QByteArray::fromPercentEncoding(QByteArray(avatar_url));

    emit success(QString(text.data()));
}
/*
 * DownloadAvatarRequest
 */
DownloadAvatarRequest::DownloadAvatarRequest(const QString& avatar_url)
    : SeafileApiRequest (QUrl(avatar_url), SeafileApiRequest::METHOD_GET)
{
}

void DownloadAvatarRequest::requestSuccess(QNetworkReply& reply)
{
    const QByteArray& avatar = reply.readAll();

    emit success(avatar);
}

