#include "controlchannel.h"

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

ControlChannel::ControlChannel() :
    Channel(ChannelManager::ROOT_CHANNEL)
{
}

ControlChannel::~ControlChannel()
{
    qDebug("### ~ControlChannel");
}

void ControlChannel::ping()
{
    if (state() == Channel::CHANNEL_CLOSED)
    {
        setState(Channel::CHANNEL_OPEN_ONGOING);
    }
    sendMessage(PING_MESSAGE);
}

void ControlChannel::receiveMessage(const QByteArray msg)
{
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "ping")
    {
        setState(Channel::CHANNEL_OPEN);
        emit pingReceived();
        sendMessage(PINGREPLY_MESSAGE);
    }
    else if (json.contains("command") && json["command"] == "pingreply")
    {
        setState(Channel::CHANNEL_OPEN);
        emit pingReceived();
        // but no reply to reply ...
    }
}
