#ifndef PLAYERCONNECTIONMANAGER_H
#define PLAYERCONNECTIONMANAGER_H

#include <QObject>
#include <QMap>

#include "server/serversidechannelmanager.h"
#include <server/websocketserver.h>
#include <server/playersession.h>

// TODO: this could be moved to folder called 'northside' i.e. towards mobapps

class PlayerChannel;


class PlayerConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerConnectionManager(WebsocketServer& websocketServer,
                                     ServerSideChannelManager& channelManager,
                                     QObject *parent = 0);
    ~PlayerConnectionManager();

signals:

public slots:
    void onNewPlayerConnection(PlayerSession session);
    void onPlayerConnectionClosed(int playerId);
    void onPlayerMessageReceived(int playerId, QString message);

private:
    WebsocketServer& _websocketServer;
    ServerSideChannelManager& _channelManager;

    QMap<int, PlayerChannel*> _channelByPlayerId;
    QMap<int, int> _playerdIdByChannelId;
};

#endif // PLAYERCONNECTIONMANAGER_H
