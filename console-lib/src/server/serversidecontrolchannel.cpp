#include "serversidecontrolchannel.h"

#include <QJsonDocument>
#include <QJsonObject>

#define LOG_AREA "ServerSideControlChannel"
#include "log/log.h"

ServerSideControlChannel::ServerSideControlChannel()
{
}

ServerSideControlChannel::~ServerSideControlChannel()
{
    TRACE("~ServerSideControlChannel");
}

bool ServerSideControlChannel::receiveMessage(const QByteArray msg)
{
    DEBUG("Processing message");
    if (ControlChannel::receiveMessage(msg))
        return true;

    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "LaunchApplication")
    {
        DEBUG("Processing message: LaunchApplication");
        if (json.contains("app_id"))
        {
            QString appId = json["app_id"].toString();
            DEBUG("Processing message: LaunchApplication with id " << appId);
            emit applicationLaunchRequested(appId);
        } else {
            WARN("'LaunchApplication'' message without 'app_id' parameter");
        }
        return true;
    }

    DEBUG("Processing message: unknown");
    return false;
}
