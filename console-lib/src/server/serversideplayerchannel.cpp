#include "serversideplayerchannel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "common/messagefactory.h"
using namespace GBerry::Console;

#define LOG_AREA "ServerSidePlayerChannel"
#include "log/log.h"

ServerSidePlayerChannel::ServerSidePlayerChannel(
        int channelId,
        PlayerMeta meta,
        QObject* qparent) :
    Channel(channelId, NULL, qparent), _playerMeta(meta), _northPartner(nullptr)
{
}

ServerSidePlayerChannel::~ServerSidePlayerChannel()
{
    TRACE("~ServerSidePlayerChannel");
    detachNorthPartner();
}

void ServerSidePlayerChannel::open()
{
    if (_state == CHANNEL_OPEN || _state == CHANNEL_OPEN_ONGOING)
        return;

    QJsonObject json;
    json["command"] = "OpenChannel";
    json["player_metadata"] = _playerMeta.json();
    QJsonDocument jsonDoc(json);

    _state = CHANNEL_OPEN_ONGOING;

    // can't use normal sendMessageToSouth() as it checks whether channel is open
    if (_southPartner)
        _southPartner->channelSendMessageToSouth(MessageFactory::createOpenChannelCommand(_playerMeta));
}

bool ServerSidePlayerChannel::receiveMessageFromSouth(const QByteArray& msg)
{
    DEBUG("receiveMessageFromSouth(): cid: " << _id);
    QJsonDocument doc(QJsonDocument::fromJson(msg));
    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "CloseChannel")
    {
        DEBUG("CloseChannel");
        processCloseReceived();
        // no responses
        return true;
    }
    else if (json.contains("command") && json["command"] == "OpenChannelAccepted")
    {
        DEBUG("OpenChannelAccepted");
        _state = CHANNEL_OPEN;
        return true;
    }
    else if (json.contains("command") && json["command"] == "PlayerMessage")
    {
        DEBUG("PlayerMessage");
        // TODO: more complex
        if (json.contains("data"))
        {
            QString msgStr;

            // TODO: encode data
            if (json["data"].isObject()) {
                msgStr.append(QJsonDocument(json["data"].toObject()).toJson());
            } else
                msgStr.append(json["data"].toString());

            // TODO: unnecessary string->qbytearray conversion?
            if (_northPartner)
                _northPartner->playerMessageFromSouth(msgStr.toLatin1());

            return true;
        } else {
            WARN("Invalid PlayerMessage, 'data' missing.");
        }
    }
    else
    {
        DEBUG("UNKNOWN MESSAGE");
    }
    // TODO: handle channel denial
    return false;
}

void ServerSidePlayerChannel::receivePlayerMessageFromNorth(const QByteArray& msg)
{
    QJsonObject json;
    json["command"] = "PlayerMessage";
    // TODO: encoding
    json["data"] = QString(msg);
    QJsonDocument doc(json);

    sendMessageToSouth(doc.toJson());
}

bool ServerSidePlayerChannel::isOpen() const
{
    return _state == CHANNEL_OPEN;
}

void ServerSidePlayerChannel::close()
{
    _isOpen = false;
    _state = CHANNEL_CLOSED;
    emit channelClosed();
}

void ServerSidePlayerChannel::attachNorthPartner(PlayerChannelNorthPartner* partner)
{
    detachNorthPartner();
    _northPartner = partner;
}

void ServerSidePlayerChannel::detachNorthPartner()
{
    if (_northPartner) delete _northPartner;
    _northPartner = nullptr;
}
