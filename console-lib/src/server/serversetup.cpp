#include "serversetup.h"

ServerSetup::ServerSetup(QObject* parent)
    : QObject(parent), tcpServer(7777)
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

}

ServerSetup::~ServerSetup()
{
    qDebug("### ~ServerSetup");
    tcpServer.close();
}

void ServerSetup::start()
{
    tcpServer.open();
}
