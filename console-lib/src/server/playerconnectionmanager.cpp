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
 
 #include "playerconnectionmanager.h"

#include <QDebug>

#include "server/serverchannelmanager.h"
#include "server/websocketserver.h"
#include "serversideplayerchannel.h"
#include "common/playermeta.h"

class PlayerConnectionManagerPrivate
{
public:
    PlayerConnectionManagerPrivate(WebsocketServer* websocketServer_,
                                   ServerChannelManager* channelManager_) :
        websocketServer(websocketServer_),
        channelManager(channelManager_) {
        Q_ASSERT(websocketServer_ != nullptr);
        Q_ASSERT(channelManager_ != nullptr);
    }

    WebsocketServer* websocketServer;
    ServerChannelManager* channelManager;

    QMap<int, ServerSidePlayerChannel*> channelByPlayerId;
    QMap<int, int> playerdIdByChannelId;
};

class PlayerChannelNorthPartnerImpl : public PlayerChannelNorthPartner
{
public:
    PlayerChannelNorthPartnerImpl(int channelId, PlayerConnectionManager* manager_) :
        PlayerChannelNorthPartner(channelId),
        manager(manager_) {}

    PlayerConnectionManager* manager;

    virtual void playerMessageFromSouth(const QByteArray &msg) override {
        manager->playerMessageFromSouth(_channelId, msg);
    }
};

PlayerConnectionManager::PlayerConnectionManager(
        WebsocketServer* websocketServer,
        ServerChannelManager* channelManager,
        QObject *parent) :
    QObject(parent),
    _d(new PlayerConnectionManagerPrivate(websocketServer, channelManager))
{
    // on new connection
    connect(_d->websocketServer, &WebsocketServer::newPlayerConnection,
            this, &PlayerConnectionManager::onNewPlayerConnection);

    connect(_d->websocketServer, &WebsocketServer::playerConnectionClosed,
            this, &PlayerConnectionManager::onPlayerConnectionClosed);

    connect(_d->websocketServer, &WebsocketServer::playerMessageReceived,
            this, &PlayerConnectionManager::onPlayerMessageReceivedFromNorth);
}

PlayerConnectionManager::~PlayerConnectionManager()
{
}

void PlayerConnectionManager::onNewPlayerConnection(PlayerSession session)
{
    int pid = session.playerId();

    PlayerMeta playerMeta(pid, session.playerName());
    ServerSidePlayerChannel* playerChannel = _d->channelManager->openPlayerChannel(playerMeta);

    _d->channelByPlayerId[pid] = playerChannel;
    _d->playerdIdByChannelId[playerChannel->channelId()] = pid;

    playerChannel->attachNorthPartner(new PlayerChannelNorthPartnerImpl(playerChannel->channelId(), this));

    // TODO: if there is no app running -> send message
}

void PlayerConnectionManager::onPlayerConnectionClosed(int playerId)
{
    // north side closed connection (either by console or client)
    if (_d->channelByPlayerId.contains(playerId))
    {
        ServerSidePlayerChannel* ch = _d->channelByPlayerId[playerId];
        _d->channelByPlayerId.remove(playerId);
        _d->playerdIdByChannelId.remove(ch->channelId());

        _d->channelManager->closePlayerChannel(ch->channelId());
    }
}

void PlayerConnectionManager::onPlayerMessageReceivedFromNorth(int playerId, QString message)
{
    // message coming from north side
    if (_d->channelByPlayerId.contains(playerId))
    {
        // TODO: this class now acts as conversion bytes -> string, not sure it is good idea ...
        //       could message be always qbytearray?
        _d->channelByPlayerId[playerId]->receivePlayerMessageFromNorth(message.toLatin1());
    }
    else
        qWarning() << "[PlayerConnectionManager] Received message from north side for unregistered player id:" << playerId;
}

void PlayerConnectionManager::playerMessageFromSouth(int channelId, const QByteArray& msg)
{
    if (_d->playerdIdByChannelId.contains(channelId))
        _d->websocketServer->sendPlayerMessage(_d->playerdIdByChannelId[channelId], QString(msg));
    else
        qWarning() << "[PlayerConnectionManager] Received msg from south from unregistered channel:" << channelId;
}
