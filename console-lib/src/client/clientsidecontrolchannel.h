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
 
 #ifndef CLIENTSIDECONTROLCHANNEL_H
#define CLIENTSIDECONTROLCHANNEL_H

#include <QObject>
#include <QScopedArrayPointer>

#include "server/icommand.h"

class QByteArray;
class ClientSideChannelPartner;
class ClientSideControlChannelPrivate;

class ClientSideControlChannel : public QObject
{
    Q_OBJECT

public:
    ClientSideControlChannel();
    ~ClientSideControlChannel();

    void requestApplicationLaunch(QString appID);
    void requestApplicationExit();

    static const int CHANNEL_ID;

    void setApplicationCode(const QString& code);

    void ping();

    bool receiveMessage(const QByteArray& msg);
    void sendMessage(const QByteArray& msg);


    void attachChannelPartner(ClientSideChannelPartner* partner);
    void detachChannelPartner();

    int channelId() const;

    bool isConnected() const;
    bool isActivated() const;

    // takes ownership of command
    void registerCommand(ICommand* cmd);

signals:
    void channelClosed();
    void pingReceived();

    void isConnectedChanged();
    void isActivatedChanged();

public slots:
    void connectionBroken();

private:
    const QScopedPointer<ClientSideControlChannelPrivate> _d;

};

#endif // CLIENTSIDECONTROLCHANNEL_H
