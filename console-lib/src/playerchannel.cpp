#include "playerchannel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


PlayerChannel::PlayerChannel(int channelId, IChannelParent* channelParent, QObject* parent) :
    Channel(channelId, channelParent, parent)
{
}

PlayerChannel::~PlayerChannel()
{
    qDebug("### ~PlayerControlChannel()");
}

bool PlayerChannel::receiveMessage(const QByteArray msg)
{
    qDebug() << "### PlayerChannel::receiveMessage(): cid: " << _id;
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "CloseChannel")
    {
        qDebug("### CloseChannel");
        closeReceived();
        // no responses
        return true;
    }
    else if (json.contains("command") && json["command"] == "OpenChannelAccepted")
    {
        qDebug("### OpenChannelAccepted");
        setState(Channel::CHANNEL_OPEN);
        return true;
    }
    else if (json.contains("command") && json["command"] == "PlayerMessage")
    {
        qDebug("### PlayerMessage");
        // TODO: more complex
        if (json.contains("data"))
        {
            // TODO: encode data
            QString msg = json["data"].toString();
            emit playerMessageReceived(channelId(), msg.toLatin1());
            return true;
        }
    }
    else
    {
        qDebug("### UNKNOWN MESSAGE");
    }
    // TODO: handle channel denial
    return false;
}

void PlayerChannel::sendPlayerMessage(QByteArray msg)
{
    QJsonObject json;
    json["command"] = "PlayerMessage";
    // TODO: encoding
    json["data"] = QString(msg);
    QJsonDocument doc(json);

    sendMessage(doc.toJson());
}
