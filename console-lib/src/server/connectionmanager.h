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
 
 #ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QMap>

class CommTcpServer;
class ServerChannelManager;
class ServerSideControlChannel;

namespace GBerry {
namespace Console {
namespace Server {
    class ApplicationRegistry;
    class ConnectionGateKeeper;
}}}

using namespace GBerry::Console::Server;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(CommTcpServer* tcpServer,
                               ApplicationRegistry* appRegistry,
                               ServerChannelManager* channelManager,
                               QObject *parent = 0);
    ~ConnectionManager();

    void closeCurrentApplication();

    // true if active connection exists
    bool activeConnection();

signals:    
    void applicationConnectionValidated(QString appId);

public slots:
    void applicationConnected(int connectionId);
    void applicationDisconnected(int connectionId);
    void incomingMessage(int connectionId, int channelId, const QByteArray msg);
    void outgoingMessageFromChannel(int connectionId, int channelId, const QByteArray msg);

    void onConnectionValidationOK(int connectionId);
    void onConnectionValidationFailed(int connectionId);
    void onOutgoingMessageFromGateKeeper(int connectionId, const QByteArray& msg);

private:
    CommTcpServer* _tcpServer;
    ApplicationRegistry* _applicationRegistry;
    ServerChannelManager* _channelManager;
    QMap<int, ConnectionGateKeeper*> _gatekeepers;
};

#endif // CONNECTIONMANAGER_H
