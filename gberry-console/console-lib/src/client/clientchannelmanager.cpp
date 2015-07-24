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
 
 #include "clientchannelmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "clientsidecontrolchannel.h"
#include "clientsideplayerchannel.h"

#define LOG_AREA "ClientChannelManager"
#include "log/log.h"

namespace {
    static const int ROOT_CHANNEL = 0;
}

class ClientChannelManagerPrivate
{
public:
    ClientChannelManagerPrivate() : controlChannel(nullptr) {}
    ~ClientChannelManagerPrivate() {
        if (controlChannel) delete controlChannel;
        foreach (auto ch, playerChannels.values()) {
            delete ch;
        }
    }

    QMap<int, ClientSidePlayerChannel*> playerChannels; // by channelId
    ClientSideControlChannel* controlChannel;
};


class ClientSideChannelPartnerImpl : public ClientSideChannelPartner
{
public:
    ClientSideChannelPartnerImpl(int channelId, ClientChannelManager* manager_) :
        ClientSideChannelPartner(channelId),
        manager(manager_) {}

    ClientChannelManager* manager;

    virtual void sendMessage(const QByteArray& msg) override {
        emit manager->outgoingMessage(_channelId, msg);
    }
    virtual void closeChannel() override {
        manager->closePlayerChannel(_channelId);
    }
};


ClientChannelManager::ClientChannelManager(QObject *parent) :
    QObject(parent),
    _d(new ClientChannelManagerPrivate)
{

}

ClientChannelManager::~ClientChannelManager()
{
    TRACE("~ClientChannelManager");   
}

QList<int> ClientChannelManager::allPlayerChannelIds()
{
    return _d->playerChannels.keys();
}

void ClientChannelManager::registerControlChannel(ClientSideControlChannel* channel)
{
    if (_d->controlChannel) delete _d->controlChannel;
    _d->controlChannel = channel;
    channel->attachChannelPartner(new ClientSideChannelPartnerImpl(channel->channelId(), this));
}

void ClientChannelManager::processMessage(int channelId, const QByteArray& msg)
{
    TRACE("processMessage(): cid=" << channelId << ", data=" << msg);

    if (channelId == ROOT_CHANNEL) {
        if (_d->controlChannel) _d->controlChannel->receiveMessage(msg);
    }
    else if (!_d->playerChannels.contains(channelId))
    {
        QJsonDocument doc(QJsonDocument::fromJson(msg));
        QJsonObject json(doc.object());
        // TODO: common impl for json validation
        if (json.contains("command") && json["command"] == "OpenChannel")
        {
            DEBUG("ClientChannelManager OpenChannel");
            // TODO: handle metadata
            if (!json.contains("player_metadata"))
                return;

            // there is PlayerMetadata -> this is Player channel
            PlayerMeta playerMeta(json["player_metadata"].toObject());

            // note controls created by ChannelManager will be deleted when
            // ChannelManager is destroyed (if not specifically before that)
            ClientSidePlayerChannel* playerControl =
                    new ClientSidePlayerChannel(channelId, playerMeta);

            _d->playerChannels[playerControl->channelId()] = playerControl;
            playerControl->attachChannelPartner(
                        new ClientSideChannelPartnerImpl(channelId, this));

            emit newPlayerChannel(playerControl, playerMeta);

            QJsonObject json;
            json["command"] = "OpenChannelAccepted"; // TODO: how denial would go, we want need to make query instead of signal (above)
            QJsonDocument jsonDoc(json);

            QByteArray msg = jsonDoc.toJson();

            emit outgoingMessage(channelId, msg);
        }
        else
        {
            WARN("Message for unhandled channel, id =" << channelId);
        }

        return;
    }
    else
    {
        ClientSidePlayerChannel* ch = _d->playerChannels[channelId];
        ch->receiveMessage(msg);
    }
}

void ClientChannelManager::closePlayerChannel(int channelId)
{
    if (_d->playerChannels.contains(channelId)) {
        _d->playerChannels[channelId]->deleteLater();
        _d->playerChannels.remove(channelId);
    } else {
        WARN("Unknown channel requested to be closed: channelId =" << channelId)
    }
}

void ClientChannelManager::closeAllPlayerChannels()
{
    foreach(auto ch, _d->playerChannels.values()) {
        ch->closeChannel();
    }
}
