/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
    DEBUG("Reopening player channel: id =" << channelId());
    if (_state == CHANNEL_OPEN || _state == CHANNEL_OPEN_ONGOING) {
        WARN("Discarded channel opening as state was:" << _state);
        return;
    }

    _state = CHANNEL_OPEN_ONGOING;

    // can't use normal sendMessageToSouth() as it checks whether channel is open
    if (_southPartner)
        _southPartner->channelSendMessageToSouth(MessageFactory::createOpenChannelCommand(_playerMeta));
}

//bool ServerSidePlayerChannel::receiveMessageFromSouth(const QByteArray& msg)
bool ServerSidePlayerChannel::processJsonMessage(const QJsonObject& json)
{
    DEBUG("receiveMessageFromSouth(): cid: " << _id);

    bool messageProcessed = Channel::processJsonMessage(json);
    if (messageProcessed)
        return true;

    //QJsonDocument doc(QJsonDocument::fromJson(msg));
    //QJsonObject json(doc.object());

    // TODO: we could have these as registered commands, but too much trouble as so integrated ...?

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
    // TODO: could there be a way to avoid parsing all messages? Also channels? What about token communication?
    QJsonParseError error;
    QJsonObject parsedJson = QJsonDocument::fromJson(msg, &error).object();
    if (!error.error != QJsonParseError::NoError && parsedJson.contains("action")
            && parsedJson["action"].toString() == "GoToMainMenu") {
        emit playerRequestedMainMenu();

    } else {
        // forward message
        QJsonObject json;
        json["command"] = "PlayerMessage";
        // TODO: encoding
        json["data"] = QString(msg);
        QJsonDocument doc(json);

        sendMessageToSouth(doc.toJson());
    }
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
