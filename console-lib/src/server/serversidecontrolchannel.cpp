#include "serversidecontrolchannel.h"

#include <QJsonObject>
#include <QJsonDocument>

#include "channelmanager.h"

namespace
{
    // TODO: now in two places -> should we put to one place (class?)
    QByteArray init(QString cmdName)
    {
        QJsonObject json;
        json["command"] = cmdName;
        QJsonDocument jsonDoc(json);
        return jsonDoc.toJson();
    }

    // TODO: need for better separation of command and reply?
    QByteArray PING_MESSAGE = init("ping");
    QByteArray PINGREPLY_MESSAGE = init("pingreply");
}

ServerSideControlChannel::ServerSideControlChannel() :
    ChannelHandler(ChannelManager::ROOT_CHANNEL)
{

}

ServerSideControlChannel::~ServerSideControlChannel()
{
    qDebug("### ~ServerSideControlChannel");
}


void ServerSideControlChannel::ping()
{
    if (state() == ChannelHandler::CHANNEL_CLOSED)
    {
        setState(ChannelHandler::CHANNEL_OPEN_ONGOING);
    }
    emit outgoingMessage(PING_MESSAGE);
}

bool ServerSideControlChannel::isConnectionValid()
{
    return state() == ChannelHandler::CHANNEL_OPEN;
}

void ServerSideControlChannel::handleMessage(const QByteArray &msg)
{
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "ping")
    {
        setState(ChannelHandler::CHANNEL_OPEN);
        emit pingReceived();
        emit outgoingMessage(PINGREPLY_MESSAGE);
    }
    else if (json.contains("command") && json["command"] == "pingreply")
    {
        setState(ChannelHandler::CHANNEL_OPEN);
        emit pingReceived();
        // but no reply to reply ...
    }
}
