#include "clientsidecontrolchannel.h"

#include "channelmanager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>


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

ClientSideControlChannel::ClientSideControlChannel() :
    ChannelHandler(ChannelManager::ROOT_CHANNEL)
{

}

ClientSideControlChannel::~ClientSideControlChannel()
{
    qDebug("### ~ClientSideControlChannel");
}

void ClientSideControlChannel::ping()
{
    if (state() == ChannelHandler::CHANNEL_CLOSED)
    {
        setState(ChannelHandler::CHANNEL_OPEN_ONGOING);
    }
    emit outgoingMessage(PING_MESSAGE);

    // TODO: how to fail if no response comes?? Now basically would go trough error handling
}

bool ClientSideControlChannel::isConnectionValid()
{
    return state() == ChannelHandler::CHANNEL_OPEN;
}

void ClientSideControlChannel::handleMessage(const QByteArray &msg)
{
    // TODO: we could place common ping func/logic to somewhere
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

// TODO: if localhost connection breaks, then reconnection needs to be done ...
