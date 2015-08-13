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
 
 #include "playersmanager.h"

#include <QMap>
#include <QJsonObject>

#include "client/clientsideplayerchannel.h"
#include "client/clientsidechannelpartners.h"

#define LOG_AREA "PlayersManager"
#include "log/log.h"

class PlayersManagerPrivate
{
public:
    PlayersManagerPrivate() {}

    QMap<int, ClientSidePlayerChannel*> _channelsByPlayerId;
    QMap<int, PlayerMeta> _meta;
    QMap<int, int> _playerIdsByChannelId;
    QMap<int, int> _channelIdsByPlayerIds;
    QMap<int, QString> _debugPlayerNamesByPlayerId;
};

class ClientSidePlayerChannelPartnerImpl : public ClientSidePlayerChannelPartner
{
public:
    ClientSidePlayerChannelPartnerImpl(int channelId, PlayersManager* manager_) :
        ClientSidePlayerChannelPartner(channelId),
        manager(manager_) {}

    PlayersManager* manager;

    virtual void playerMessage(const QByteArray& msg) override {
        manager->playerMessage(_channelId, msg);
    }

    virtual void playerExit() override {
        manager->playerExit(_channelId);
    }
};


PlayersManager::PlayersManager(QObject *parent) :
    QObject(parent),
    _d(new PlayersManagerPrivate)
{
}

PlayersManager::~PlayersManager()
{
}

int PlayersManager::numberOfPlayers() const
{
    return _d->_channelIdsByPlayerIds.size() + _d->_debugPlayerNamesByPlayerId.size();
}

QList<int> PlayersManager::playerIds() const
{
    QList<int> ids = _d->_meta.keys();
    ids << _d->_debugPlayerNamesByPlayerId.keys();
    return ids;
}

QString PlayersManager::playerName(int playerId) const
{
    if (_d->_meta.contains(playerId))
         return _d->_meta[playerId].playerName();

    if (_d->_debugPlayerNamesByPlayerId.contains(playerId))
        return _d->_debugPlayerNamesByPlayerId[playerId];

    return QString("UNKNOWN");
}

bool PlayersManager::isGuest(int playerId) const
{
    return playerId < -1; // guest ids are -2 and downwards
}

void PlayersManager::newPlayer(ClientSidePlayerChannel* channel, PlayerMeta metadata)
{
    Q_ASSERT(channel);

    int channelId = channel->channelId();

    if (!_d->_playerIdsByChannelId.contains(channelId))
    {
        int pid = metadata.playerId();
        _d->_meta[pid] = metadata;
        _d->_playerIdsByChannelId[channelId] = metadata.playerId();
        _d->_channelIdsByPlayerIds[pid] = channelId;
        _d->_channelsByPlayerId[pid] = channel;

        channel->attachPlayerChannelPartner(
                    new ClientSidePlayerChannelPartnerImpl(channelId, this));

        emit playerIn(pid, metadata.playerName());
        emit numberOfPlayersChanged();
    }
}

void PlayersManager::playerExit(int channelId)
{
    if (_d->_playerIdsByChannelId.contains(channelId))
    {
        int playerId = _d->_playerIdsByChannelId[channelId];
        _d->_playerIdsByChannelId.remove(channelId);
        _d->_channelIdsByPlayerIds.remove(playerId);
        _d->_channelsByPlayerId.remove(playerId);
        PlayerMeta meta = _d->_meta[playerId];
        _d->_meta.remove(playerId);

        // we discard metadata of disconnectectedplayer,
        // just provide name on exit signal
        emit playerOut(playerId, meta.playerName());
        emit numberOfPlayersChanged();
    }
}

void PlayersManager::sendPlayerMessage(int playerId, const QByteArray msg)
{
    //DEBUG("Sending for player:" << playerId);
    if (_d->_channelsByPlayerId.contains(playerId)) {
        _d->_channelsByPlayerId[playerId]->sendPlayerMessage(msg);
    } else if (_d->_debugPlayerNamesByPlayerId.contains(playerId)) {
        //DEBUG("Sending for debug player: " << playerId);
        emit debugPlayerMessageReceived(playerId, msg);
    }
}

void PlayersManager::sendAllPlayersMessage(QByteArray msg)
{
    //DEBUG("Sending for all players");
    foreach(int pid, _d->_channelIdsByPlayerIds.keys()) {
        _d->_channelsByPlayerId[pid]->sendPlayerMessage(msg);
    }

    foreach(int pid, _d->_debugPlayerNamesByPlayerId.keys()) {
        //DEBUG("Sending for debug player: " << pid);
        emit debugPlayerMessageReceived(pid, msg);
    }
}

int PlayersManager::registerDebugPlayer(const QString& playerName)
{
    // just picking id base number far enough
    int newPid = 100000 + _d->_debugPlayerNamesByPlayerId.size();
    _d->_debugPlayerNamesByPlayerId[newPid] = playerName;

    DEBUG("Registered debug player: name =" << playerName << ", id =" << newPid);
    emit numberOfPlayersChanged();
    return newPid;
}

void PlayersManager::sendDebugPlayerMessage(int playerId, const QByteArray& msg)
{
    // this message is coming from debug client to application
    emit playerMessageReceived(playerId, msg);
}

void PlayersManager::playerMessage(int channelId, const QByteArray& msg)
{
    if (_d->_playerIdsByChannelId.contains(channelId))
    {
        emit playerMessageReceived(_d->_playerIdsByChannelId[channelId], msg);
    }
}
