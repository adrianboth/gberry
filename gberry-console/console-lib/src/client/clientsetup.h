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
 
 #ifndef CLIENTSETUP_H
#define CLIENTSETUP_H

#include <QObject>
#include "commtcpclient.h"
#include "client/4qml/playersmanager.h"
#include "client/clientchannelmanager.h"
#include "client/clientsidecontrolchannel.h"


class ClientSetup : public QObject
{
    Q_OBJECT

public:
    ClientSetup(QObject* parent = 0);
    ~ClientSetup();

    void start();

    CommTcpClient tcpClient;
    ClientChannelManager channelManager;
    ClientSideControlChannel* controlChannel;
    PlayersManager playersManager;

public slots:
    void connectionEstablished();
    void connectionBroken();

};

#endif // CLIENTSETUP_H
