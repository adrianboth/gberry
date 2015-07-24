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
 
 #ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "client/clientsideplayerchannel.h"
#include "common/playermeta.h"

class PlayersManagerPrivate;
class ClientSidePlayerChannelPartnerImpl;


class PlayersManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int numberOfPlayers READ numberOfPlayers NOTIFY numberOfPlayersChanged)



public:
    explicit PlayersManager(QObject *parent = 0);
    ~PlayersManager();

    int numberOfPlayers() const;
    Q_INVOKABLE QList<int> playerIds() const;
    Q_INVOKABLE QString playerName(int playerId) const;

    // TODO: for now player id matches to channel id, but later it should be different
    //       as player might disconnect for awhile (in phone, e.g.) but then come
    //       back and channel id will change -> game might have kept some information
    //       stored and player can continue if player ids match

    Q_INVOKABLE void sendPlayerMessage(int playerId, const QByteArray msg);
    Q_INVOKABLE void sendAllPlayersMessage(QByteArray msg);


signals:
    void playerIn(int playerId, QString playerName);
    void playerOut(int playerId, QString playerName);
    void numberOfPlayersChanged();
    void playerMessageReceived(int playerId, QByteArray msg); // TODO: might need to be string

public slots:
    void newPlayer(ClientSidePlayerChannel* channel, PlayerMeta metadata);

protected:
    friend class ClientSidePlayerChannelPartnerImpl;

    // TODO: Q.object to hide these
    // from ClientSidePlayerChannelPartner
    void playerMessage(int channelId, const QByteArray& msg);
    void playerExit(int channelId);

private:
    const QScopedPointer<PlayersManagerPrivate> _d;
};


#endif // PLAYERSMANAGER_H
