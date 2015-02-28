#include "playersmanager.h"
#include "playerchannel.h"


PlayersManager::PlayersManager(QObject *parent) :
    QObject(parent), _playerCount(0)
{

}

PlayersManager::~PlayersManager()
{
    qDebug("### ~PlayersManager");
}

int PlayersManager::numberOfPlayers() const
{
    return _playerCount;
}

QList<int> PlayersManager::playerIds() const
{
    return _meta.keys();
}

QString PlayersManager::playerName(int playerId) const
{
    if (_meta.contains(playerId))
         return _meta[playerId].playerName;

    return QString("UNKNOWN");
}

void PlayersManager::newPlayer(PlayerChannel* channel, PlayerMetadata metadata)
{
    Q_ASSERT(channel);

    int channelId = channel->channelId();

    if (!_playerIdsByChannelId.contains(channelId))
    {
        _meta[metadata.playerId] = metadata;
        _playerIdsByChannelId[channelId] = metadata.playerId;
        _channelIdsByPlayerIds[metadata.playerId] = channelId;
        _channelsByPlayerId[metadata.playerId] = channel;

        connect(channel, &PlayerChannel::playerMessageReceived,
                this,    &PlayersManager::playerMessage);

        _playerCount++; // TODO: could be removed
        emit playerIn(metadata.playerId);
    }
}

void PlayersManager::playerExit(int channelId)
{
    if (_playerIdsByChannelId.contains(channelId))
    {
        int playerId = _playerIdsByChannelId[channelId];
        _playerIdsByChannelId.remove(channelId);
        _channelIdsByPlayerIds.remove(playerId);
        _channelsByPlayerId.remove(playerId);
        _meta.remove(playerId);
        _playerCount--;

        // TODO: would game need meta data still after exit?
        //       - principle yes (if he will come back) but whose responsibility is to store info?
        emit playerOut(playerId);
    }
}

void PlayersManager::sendPlayerMessage(int playerId, QByteArray msg)
{
    if (_channelsByPlayerId.contains(playerId))
    {
        _channelsByPlayerId[playerId]->sendPlayerMessage(msg);
    }
}

void PlayersManager::playerMessage(int channelId, QByteArray msg)
{
    if (_playerIdsByChannelId.contains(channelId))
    {
        emit playerMessageReceived(_playerIdsByChannelId[channelId], msg);
    }
}
