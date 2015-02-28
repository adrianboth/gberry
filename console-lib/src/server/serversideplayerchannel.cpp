#include "serversideplayerchannel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


ServerSidePlayerChannel::ServerSidePlayerChannel(
        int channelId,
        PlayerMetadata meta,
        QObject* qparent) :
    PlayerChannel(channelId, NULL, qparent), _playerMeta(meta)
{
}

ServerSidePlayerChannel::~ServerSidePlayerChannel()
{
}

void ServerSidePlayerChannel::open()
{
    QJsonObject json;
    json["command"] = "OpenChannel";
    json["player_metadata"] = _playerMeta.json();
    QJsonDocument jsonDoc(json);

    setState(Channel::CHANNEL_OPEN_ONGOING);
    qDebug() << "### SENDING OPEN CHANNEL FROM SERVER";
    sendMessage(jsonDoc.toJson());
}


void ServerSidePlayerChannel::reopen()
{

    if (_state == Channel::CHANNEL_OPEN || _state == Channel::CHANNEL_OPEN_ONGOING)
        return;

    open();
}
