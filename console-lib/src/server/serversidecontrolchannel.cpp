#include "serversidecontrolchannel.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "messagefactory.h"

using namespace GBerry::Console;

#define LOG_AREA "ServerSideControlChannel"
#include "log/log.h"


const int ServerSideControlChannel::ROOT_CHANNEL_ID = 0;


ServerSideControlChannel::ServerSideControlChannel() :
    Channel(ROOT_CHANNEL_ID)
{
}

ServerSideControlChannel::~ServerSideControlChannel()
{
    TRACE("~ServerSideControlChannel");
}

bool ServerSideControlChannel::processJsonMessage(const QJsonObject &json)
{
    DEBUG("Processing message");

    if (Channel::processJsonMessage(json))
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

    DEBUG("Processing message: unknown");
    return false;
}

void ServerSideControlChannel::pingSouth()
{
    sendMessageToSouth(MessageFactory::createPingCommand());
}

/*
bool ServerSideControlChannel::receiveMessageFromSouth(const QByteArray& msg)
{
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "ping")
    {
        emit pingSouthReceived();
        sendMessageToSouth(PINGREPLY_MESSAGE);
        return true;
    }
    else if (json.contains("command") && json["command"] == "pingreply")
    {
        emit pingSouthReceived();
        // but no reply to reply ...
        return true;
    }

    // not known by us
    return Channel::receiveMessageFromSouth(msg);
}
*/
