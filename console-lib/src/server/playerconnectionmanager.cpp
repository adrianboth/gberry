#include "playerconnectionmanager.h"

#include <QDebug>

#include "playerchannel.h"


PlayerConnectionManager::PlayerConnectionManager(
        WebsocketServer& websocketServer,
        ServerSideChannelManager& channelManager,
        QObject *parent) :
    QObject(parent), _websocketServer(websocketServer), _channelManager(channelManager)
{
    // on new connection
    connect(&_websocketServer, &WebsocketServer::newPlayerConnection,
            this, &PlayerConnectionManager::onNewPlayerConnection);

    connect(&_websocketServer, &WebsocketServer::playerConnectionClosed,
            this, &PlayerConnectionManager::onPlayerConnectionClosed);

    connect(&_websocketServer, &WebsocketServer::playerMessageReceived,
            this, &PlayerConnectionManager::onPlayerMessageReceived);
}

PlayerConnectionManager::~PlayerConnectionManager()
{

}

void PlayerConnectionManager::onNewPlayerConnection(PlayerSession session)
{
    int pid = session.playerId();

    PlayerMetadata playerMeta(pid, session.playerName());
    PlayerChannel* playerChannel = _channelManager.openPlayerChannel(playerMeta);

    _channelByPlayerId[pid] = playerChannel;
    _playerdIdByChannelId[playerChannel->channelId()] = pid;

    connect(playerChannel, &PlayerChannel::playerMessageReceived,
            [this] (int channelId, const QByteArray msg)
    {
        if (_playerdIdByChannelId.contains(channelId))
            _websocketServer.sendPlayerMessage(_playerdIdByChannelId[channelId], QString(msg));
        else
            qWarning() << "[PlayerConnectionManager] Received msg from south from unregistered channel:" << channelId;
    });

    // TODO: if there is no app running -> send message
}

void PlayerConnectionManager::onPlayerConnectionClosed(int playerId)
{
    // north side closed connection (either by console or client)
    if (_channelByPlayerId.contains(playerId))
    {
        PlayerChannel* ch = _channelByPlayerId[playerId];
        _channelByPlayerId.remove(playerId);
        _playerdIdByChannelId.remove(ch->channelId());

        _channelManager.closeChannel(ch->channelId());
        _channelManager.unregisterChannel(ch->channelId());
        delete ch; // TODO: is this really right?
    }
}

void PlayerConnectionManager::onPlayerMessageReceived(int playerId, QString message)
{
    // message coming from north side
    if (_channelByPlayerId.contains(playerId))
    {
        // TODO: this class now acts as conversion bytes -> string, not sure it is good idea ...
        //       could message be always qbytearray?
        _channelByPlayerId[playerId]->sendPlayerMessage(message.toLatin1());
    }
    else
        qWarning() << "[PlayerConnectionManager] Received message from north side for unregistered player id:" << playerId;
}
