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

bool ServerSideControlChannel::processJsonMessage(const QJsonObject &json)
{
    DEBUG("Processing message");

    if (ControlChannel::processJsonMessage(json))
        return true;

    if (!json.contains("command"))
        return false;

    if (json["command"] == "LaunchApplication")
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

    /* TODO: implement as command
    if (json["command"] == "QueryLocalApplications")
    {
        DEBUG("Processing message: QueryLocalApplications");

        // TODO: get apps and marshal them
    }
    */

    DEBUG("Processing message: unknown");
    return false;
}
