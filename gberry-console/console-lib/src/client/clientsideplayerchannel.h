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
 
 #ifndef CLIENTSIDEPLAYERCHANNEL_H
#define CLIENTSIDEPLAYERCHANNEL_H

#include <QObject>
#include <QByteArray>

#include "common/playermeta.h"

class ClientSideChannelPartner;
class ClientSidePlayerChannelPartner;

class ClientSidePlayerChannel : public QObject
{
    Q_OBJECT

public:
    ClientSidePlayerChannel(int channelId, PlayerMeta& metadata);
    ~ClientSidePlayerChannel();

    void attachChannelPartner(ClientSideChannelPartner* partner);
    void detachChannelPartner();

    void attachPlayerChannelPartner(ClientSidePlayerChannelPartner* partner);
    void detachPlayerChannelPartner();

    int channelId();

    void closeChannel();

    // called when new message from server
    bool receiveMessage(const QByteArray& msg);

    // called when message to a player is wanted to be sent
    void sendPlayerMessage(const QByteArray& msg);

protected:
    int _id;

private:
    ClientSideChannelPartner* _channelPartner;
    ClientSidePlayerChannelPartner* _playerPartner;
    PlayerMeta _playerMeta;
};

#endif // CLIENTSIDEPLAYERCHANNEL_H
