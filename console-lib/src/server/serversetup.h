#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QObject>
#include "server/commtcpserver.h"
#include "server/serverchannelmanager.h"
#include "server/serversidecontrolchannel.h"
#include "server/connectionmanager.h"
#include "server/playerconnectionmanager.h"

#include <server/playersessionmanager.h>
#include <server/consolerestserver.h>
#include <server/websocketserver.h>

#include "server/applicationregistry.h"


class ServerSetup : public QObject
{
    Q_OBJECT

public:
    ServerSetup(QObject* parent = 0);
    ~ServerSetup();

    // creates connections
    void setup();
    void start();
    void use(ChannelFactory* factory);

    CommTcpServer tcpServer;
    ServerChannelManager* channelManager;
    ConnectionManager* connectionManager;

    PlayerSessionManager sessionManager;
    ConsoleRESTServer restServer;
    WebsocketServer websocketServer;

    PlayerConnectionManager* playerConnectionManager;

    ApplicationRegistry applicationRegistry;
    ChannelFactory* channelFactory;

protected:
    void startSouthSide();
    void startNorthSide();
};

#endif // SERVERSETUP_H
