#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QObject>
#include "server/commtcpserver.h"
#include "server/serversidechannelmanager.h"
#include "server/serversidecontrolchannel.h"
#include "server/connectionmanager.h"
#include "server/playerconnectionmanager.h"

#include <server/playersessionmanager.h>
#include <server/consolerestserver.h>
#include <server/websocketserver.h>


class ServerSetup : public QObject
{
    Q_OBJECT

public:
    ServerSetup(QObject* parent = 0);
    ~ServerSetup();

    void start();

    void startSouthSide();
    void startNorthSide();

    CommTcpServer tcpServer;
    ServerSideChannelManager channelManager;
    ServerSideControlChannel controlChannel;
    ConnectionManager* connectionManager;

    PlayerSessionManager sessionManager;
    ConsoleRESTServer restServer;
    WebsocketServer websocketServer;

    PlayerConnectionManager playerConnectionManager;
};

#endif // SERVERSETUP_H
