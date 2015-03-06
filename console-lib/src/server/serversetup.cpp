#include "serversetup.h"

ServerSetup::ServerSetup(QObject* parent)
    : QObject(parent),
      tcpServer(7777),
      sessionManager(),
      restServer(sessionManager),
      websocketServer(&sessionManager),
      playerConnectionManager(websocketServer, channelManager)
{
    connectionManager = new ConnectionManager(&tcpServer, &channelManager, &controlChannel, this);

    // ConnectionManager works as adapter
    connect(&channelManager, &ChannelManager::outgoingMessage,
            connectionManager, &ConnectionManager::outgoingMessageFromChannel);

    connect(&tcpServer, &CommTcpServer::received,
            connectionManager, &ConnectionManager::incomingMessage);

    // TODO: these could be moved to ConnectionManager
    connect(&tcpServer, &CommTcpServer::connected,
            connectionManager, &ConnectionManager::applicationConnected);

    connect(&tcpServer, &CommTcpServer::disconnected,
            connectionManager, &ConnectionManager::applicationDisconnected);

    connect(&controlChannel,   &ServerSideControlChannel::pingReceived,
            connectionManager, &ConnectionManager::pingOK);

    channelManager.registerChannel(&controlChannel);

    // -- setup north side

    // ... nothing specific yet
}

ServerSetup::~ServerSetup()
{
    qDebug("### ~ServerSetup");
    tcpServer.close();
}

void ServerSetup::start()
{
    startSouthSide();
    startNorthSide();
}

void ServerSetup::startSouthSide()
{
    tcpServer.open();
}


void ServerSetup::startNorthSide()
{
    websocketServer.start();
}
