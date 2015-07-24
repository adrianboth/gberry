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
 
 #include "clientsideplayerchannel.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "clientsidechannelpartners.h"

#define LOG_AREA "ClientSidePlayerChannel"
#include "log/log.h"

ClientSidePlayerChannel::ClientSidePlayerChannel(int channelId, PlayerMeta& metdata) :
    _id(channelId),
    _channelPartner(nullptr),
    _playerPartner(nullptr),
    _playerMeta(metdata)
{
}

ClientSidePlayerChannel::~ClientSidePlayerChannel()
{
    detachChannelPartner();
    detachPlayerChannelPartner();
}

void ClientSidePlayerChannel::attachChannelPartner(ClientSideChannelPartner* partner)
{
    _channelPartner = partner;
}

void ClientSidePlayerChannel::detachChannelPartner()
{
    if (_channelPartner) delete _channelPartner;
    _channelPartner = nullptr;
}

void ClientSidePlayerChannel::attachPlayerChannelPartner(ClientSidePlayerChannelPartner* partner)
{
    _playerPartner = partner;
}

void ClientSidePlayerChannel::detachPlayerChannelPartner()
{
    if (_playerPartner) delete _playerPartner;
    _playerPartner = nullptr;
}

int ClientSidePlayerChannel::channelId()
{
    return _id;
}

void ClientSidePlayerChannel::sendPlayerMessage(const QByteArray& msg)
{
    QJsonObject json;
    json["command"] = "PlayerMessage";
    json["data"] = QString(msg);
    QJsonDocument doc(json);

    if (_channelPartner) {
        _channelPartner->sendMessage(doc.toJson());
    }
}

bool ClientSidePlayerChannel::receiveMessage(const QByteArray& msg)
{
    QJsonParseError errors;

    QJsonDocument doc(QJsonDocument::fromJson(msg, &errors));
    if (errors.error != QJsonParseError::NoError) {
        WARN("Failed to parse json message: error =" << errors.errorString() << ", offset =" << errors.offset << ", message =" << msg);
        return false;
    }

    QJsonObject json(doc.object());

    if (json.contains("command") && json["command"] == "CloseChannel")
    {
        DEBUG("CloseChannel received");
        closeChannel();
        // no responses
        return true;
    }
    else if (json.contains("command") && json["command"] == "PlayerMessage")
    {
        DEBUG("PlayerMessage received");
        // TODO: more complex
        if (json.contains("data"))
        {
            QString msgStr;

            // TODO: encode data
            if (json["data"].isObject()) {
                msgStr.append(QJsonDocument(json["data"].toObject()).toJson());
            } else
                msgStr.append(json["data"].toString());

            if (_playerPartner)
                _playerPartner->playerMessage(msgStr.toLatin1());

            return true;
        }
    }
    else
    {
        DEBUG("UNKNOWN MESSAGE");
    }

    return false;
}

void ClientSidePlayerChannel::closeChannel()
{
    if (_playerPartner) _playerPartner->playerExit();
    if (_channelPartner) _channelPartner->closeChannel();
}
