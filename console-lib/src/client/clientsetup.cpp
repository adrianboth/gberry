/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "clientsetup.h"

ClientSetup::ClientSetup(QObject* parent) :
    QObject(parent), tcpClient(7777), controlChannel(nullptr)
{
    QObject::connect(&tcpClient,      &CommTcpClient::received,
                     &channelManager, &ClientChannelManager::processMessage);

    QObject::connect(&channelManager, &ClientChannelManager::outgoingMessage,
                     &tcpClient,      &CommTcpClient::write);

    QObject::connect(&tcpClient,    &CommTcpClient::connected,
                     this,          &ClientSetup::connectionEstablished);

    QObject::connect(&tcpClient,    &CommTcpClient::disconnected,
                     this,          &ClientSetup::connectionBroken);

    controlChannel = new ClientSideControlChannel;
    channelManager.registerControlChannel(controlChannel);

    QObject::connect(&tcpClient,     &CommTcpClient::disconnected,
                     controlChannel, &ClientSideControlChannel::connectionBroken);

    QObject::connect(&channelManager, &ClientChannelManager::newPlayerChannel,
                     &playersManager, &PlayersManager::newPlayer);

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
    // controlChannel.ping(); // TODO: nowadays server side initiates
}

void ClientSetup::connectionBroken()
{
    // all player channels will be destroyed
    channelManager.closeAllPlayerChannels();

    // TODO: should we try to periodically reopen ... more like dev time setup
    //       if comms has been restarted
}
