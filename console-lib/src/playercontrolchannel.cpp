#include "playercontrolchannel.h"

#include <QJsonDocument>
#include <QJsonObject>


PlayerControlChannel::PlayerControlChannel(int channelId, QObject* parent) :
    ChannelHandler(channelId, parent)
{

}

PlayerControlChannel::~PlayerControlChannel()
{
    qDebug("### ~PlayerControlChannel()");

}

void PlayerControlChannel::handleMessage(const QByteArray &msg)
{
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "CloseChannel")
    {
        setState(ChannelHandler::CHANNEL_CLOSED);
        emit closeReceived(this);
        // no responses

        // having a parent means that ChannelManager created us
        if (parent())
            this->deleteLater();
    }
    else if (json.contains("command") && json["command"] == "OpenChannelAccepted")
    {
        setState(ChannelHandler::CHANNEL_OPEN);
    }

    // TODO: handle channel denial
}

