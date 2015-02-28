#include "clientsetup.h"

ClientSetup::ClientSetup(QObject* parent) :
    QObject(parent), tcpClient(7777)
{
    QObject::connect(&tcpClient,      &CommTcpClient::received,
                     &channelManager, &ChannelManager::processMessage);

    QObject::connect(&channelManager, &ChannelManager::outgoingMessage,
                     &tcpClient,      &CommTcpClient::write);

    QObject::connect(&tcpClient,    &CommTcpClient::connected,
                     this,          &ClientSetup::connectionEstablished);

    QObject::connect(&tcpClient,    &CommTcpClient::disconnected,
                     this,          &ClientSetup::connectionBroken);

    channelManager.registerChannel(&controlChannel);

    QObject::connect(&channelManager, &ClientSideChannelManager::newPlayerChannel,
                     &playersManager, &PlayersManager::newPlayer);

    QObject::connect(&channelManager, &ChannelManager::channelClosed,
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

void ClientSetup::connectionEstablished()
{
    // socket connection ok
    controlChannel.ping();
}

void ClientSetup::connectionBroken()
{
    // all player channels will be destroyed
    foreach (int cid, channelManager.allChannelIds())
    {
        if (cid != ChannelManager::ROOT_CHANNEL)
        {
            Channel* handler = channelManager.unregisterChannel(cid);
            delete handler;
            playersManager.playerExit(cid);
        }
    }
}
