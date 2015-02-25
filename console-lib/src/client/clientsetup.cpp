#include "clientsetup.h"

ClientSetup::ClientSetup(QObject* parent) :
    QObject(parent), tcpClient(7777)
{
    QObject::connect(&tcpClient,      &CommTcpClient::received,
                     &channelManager, &ChannelManager::processMessage);

    QObject::connect(&channelManager, &ChannelManager::outgoingMessage,
                     &tcpClient,      &CommTcpClient::write);

    channelManager.registerHandler(&controlChannel);

    QObject::connect(&channelManager, &ChannelManager::newChannel, // TODO: channel state is not connect until connected message is received
                     &playersManager, &PlayersManager::newPlayer);

    QObject::connect(&channelManager, &ChannelManager::channelClosed, // TODO: channel state is not connect until connected message is received
                     &playersManager, &PlayersManager::playerExit);
}

ClientSetup::~ClientSetup()
{
    qDebug("### ~ClientSetup");
    tcpClient.close();
}

void ClientSetup::start()
{
    tcpClient.open();
}
