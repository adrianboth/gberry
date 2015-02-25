#include "serversetup.h"

ServerSetup::ServerSetup(QObject* parent)
    : QObject(parent), tcpServer(7777)
{
    connectionManager = new ConnectionManager(&tcpServer, &channelManager, &controlChannel, this);

    // we discard connectionId, in future it might have meaning
    connect(&tcpServer, &CommTcpServer::received,
            [this] (int connectionId, int channelId, const QByteArray& msg) {
        Q_UNUSED(connectionId);
        this->channelManager.processMessage(channelId, msg);
    });

    // ConnectionManager works as adapter
    connect(&channelManager, &ChannelManager::outgoingMessage,
            connectionManager, &ConnectionManager::outgoingMessageFromChannel);

    connect(&tcpServer, &CommTcpServer::received,
            connectionManager, &ConnectionManager::incomingMessage);

    connect(&tcpServer, &CommTcpServer::connected,
            connectionManager, &ConnectionManager::applicationConnected);

    connect(&tcpServer, &CommTcpServer::disconnected,
            connectionManager, &ConnectionManager::applicationDisconnected);

    channelManager.registerHandler(&controlChannel);

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
