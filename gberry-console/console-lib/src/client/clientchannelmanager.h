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
 
 #ifndef CLIENTCHANNELMANAGER_H
#define CLIENTCHANNELMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QScopedPointer>

#include "clientsidechannelpartners.h"
#include "common/playermeta.h"

class ClientSidePlayerChannel;
class ClientSideControlChannel;
class ClientChannelManagerPrivate;
class ClientSideChannelPartnerImpl;


class ClientChannelManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientChannelManager(QObject *parent = 0);
    ~ClientChannelManager();

    void registerControlChannel(ClientSideControlChannel* channel);

    QList<int> allPlayerChannelIds();
    void closePlayerChannel(int channelId);
    void closeAllPlayerChannels();

signals:
    // send messages towards server
    void outgoingMessage(int channelId, const QByteArray msg);

    void newPlayerChannel(ClientSidePlayerChannel* channel, PlayerMeta playerMeta);

public slots:
    virtual void processMessage(int channelId, const QByteArray& msg);

private:
    friend class ClientSideChannelPartnerImpl;
    const QScopedPointer<ClientChannelManagerPrivate> _d;
};
#endif // CLIENTCHANNELMANAGER_H
