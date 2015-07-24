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
 
 #include "serversetup.h"

#define LOG_AREA "ServerSetup"
#include "log/log.h"

ServerSetup::ServerSetup(QObject* parent)
    : QObject(parent),
      tcpServer(7777),
      channelManager(nullptr),
      connectionManager(nullptr),
      sessionManager(),
      restServer(sessionManager),
      websocketServer(&sessionManager),
      playerConnectionManager(nullptr),
      channelFactory(nullptr),
      _setupDone(false)
{
}

void ServerSetup::setup()
{
    if (_setupDone) return;

    Q_ASSERT(channelFactory); // use() needs to be used
    channelManager = new ServerChannelManager(channelFactory, this);
    connectionManager = new ConnectionManager(&tcpServer, &applicationRegistry, channelManager, this);
    playerConnectionManager = new PlayerConnectionManager(&websocketServer, channelManager, this);

    DEBUG("Connecting signals");
    // ConnectionManager works as adapter
    connect(channelManager, &ServerChannelManager::outgoingMessageToSouth,
            connectionManager, &ConnectionManager::outgoingMessageFromChannel);

    connect(&tcpServer, &CommTcpServer::received,
            connectionManager, &ConnectionManager::incomingMessage);

    // TODO: these could be moved to ConnectionManager
    connect(&tcpServer, &CommTcpServer::connected,
            connectionManager, &ConnectionManager::applicationConnected);

    connect(&tcpServer, &CommTcpServer::disconnected,
            connectionManager, &ConnectionManager::applicationDisconnected);

    // -- setup north side

    // ... nothing specific yet

    DEBUG("Setup OK");
    _setupDone = true;
}

ServerSetup::~ServerSetup()
{
    TRACE("~ServerSetup");
    tcpServer.close();
}

void ServerSetup::start()
{
    setup();
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

void ServerSetup::use(ChannelFactory* factory)
{
    channelFactory = factory;
}
