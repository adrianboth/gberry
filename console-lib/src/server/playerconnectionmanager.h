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
 
 #ifndef PLAYERCONNECTIONMANAGER_H
#define PLAYERCONNECTIONMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QMap>

#include <server/playersession.h>

// TODO: this could be moved to folder called 'northside' i.e. towards mobapps

class WebsocketServer;
class ServerChannelManager;
class ServerSidePlayerChannel;
class PlayerConnectionManagerPrivate;
class PlayerChannelNorthPartnerImpl;

/**
 * @brief The PlayerConnectionManager class
 *
 * On north side of server communication:
 *
 * ConsoleRESTServer listens HTTP rest calls. Mobapp clients register and get
 * a code that can be used to open websocket connection. That connection
 * forms a player channel by PlayerConnectionManager.
 *
 * Once connection is established, PlayerConnectionManager is responsible
 * for managing it on north side: messages and disconnects.
 *
 * Note that south side never disconnects, instead player connections are
 * kept open and reopened on the south side when that is approriate.
 *
 */
class PlayerConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit PlayerConnectionManager(WebsocketServer* websocketServer,
                                     ServerChannelManager* channelManager,
                                     QObject *parent = 0);
    ~PlayerConnectionManager();

public slots:
    void onNewPlayerConnection(PlayerSession session);
    void onPlayerConnectionClosed(int playerId);
    void onPlayerMessageReceivedFromNorth(int playerId, QString message);

protected:
    friend class PlayerChannelNorthPartnerImpl;
    void playerMessageFromSouth(int channelId, const QByteArray& msg);

private:
    const QScopedPointer<PlayerConnectionManagerPrivate> _d;

};

#endif // PLAYERCONNECTIONMANAGER_H
