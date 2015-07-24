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
private:
    bool _setupDone;
};

#endif // SERVERSETUP_H
