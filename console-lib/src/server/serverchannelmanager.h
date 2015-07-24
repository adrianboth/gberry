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
 
 #ifndef SERVERCHANNELMANAGER_H
#define SERVERCHANNELMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "common/playermeta.h"

namespace GBerry {
namespace Console {
namespace Server {
    class ChannelFactory;
}}}

using namespace GBerry::Console::Server;

class ServerSidePlayerChannel;
class ServerSideControlChannel;
class ServerChannelManagerPrivate;
class PlayerChannelSouthPartner;

class ServerChannelManager : public QObject
{
    Q_OBJECT

public:
    ServerChannelManager(ChannelFactory* factory, QObject* parent = 0);
    ~ServerChannelManager();

    int nextFreeChannelId();
    ServerSidePlayerChannel* openPlayerChannel(PlayerMeta metadata);
    ServerSideControlChannel* openControlChannel(int connectionId);

    // If channel is open then it is closed before unattaching
    // ChannelHandler from channel.
    void closePlayerChannel(int channelId);
    void closeControlChannel(int connectionId);

    QList<int> allPlayerChannelIds();

    void activateConnection(int connectionId);
    // between launching apps there can be periods when no application is active
    void deactivateConnection(int connectionId);
    bool hasActiveConnection();

public slots:
    void applicationClosed(int connectionId); // connection towards client closed
    void processMessageFromSouth(int connectionId, int channelId, const QByteArray& msg);

signals:
    void outgoingMessageToSouth(int connectionId, int channelId, const QByteArray msg);

private:
    friend class PlayerChannelSouthPartnerImpl;
    const QScopedPointer<ServerChannelManagerPrivate> _d;
};

#endif // SERVERCHANNELMANAGER_H
