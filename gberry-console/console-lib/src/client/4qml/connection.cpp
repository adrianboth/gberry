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
 
 #include "connection.h"

#include <QJsonObject>
#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

class ConnectionPrivate
{
public:
    explicit ConnectionPrivate(
            IConnectionCommunication* communication_,
            ClientSideControlChannel* controlChannel_,
            Connection* q_) :
        communication(communication_),
        controlChannel(controlChannel_),
        q(q_) {
        QObject::connect(communication, &IConnectionCommunication::messageReceived,
                        [this] (const QJsonObject& msg) { this->onMessageReceived(msg); });

        QObject::connect(controlChannel, &ClientSideControlChannel::isConnectedChanged,
                        [&] () { this->onConnectedChanged(); });

       QObject::connect(controlChannel, &ClientSideControlChannel::isActivatedChanged,
                        [&] () { this->onActivatedChanged(); });
    }

    IConnectionCommunication* communication;
    ClientSideControlChannel* controlChannel;
    Connection* q;
    bool isHeadServerConnected{false};

    void onActivatedChanged()
    {
        // now we can query headserver connection status
        QJsonObject cmd;
        cmd["command"] = "QueryHeadServerStatus";
        controlChannel->sendMessage(QJsonDocument(cmd).toJson());
        emit q->isActivatedChanged();
    }

    void onConnectedChanged()
    {
        emit q->isConnectedChanged();
    }

    void onHeadServerConnectedChanged(bool connected)
    {
        isHeadServerConnected = connected;
        emit q->isHeadServerConnectedChanged();
    }

    void onMessageReceived(const QJsonObject& json) {
        bool currentConnected = json["status"].toString() == "connected" ? true : false;
        // TODO: needed at all?
        //bool oldConnected = "connected" ? true : false;
        if (isHeadServerConnected != currentConnected) {
            onHeadServerConnectedChanged(currentConnected);
        }
    }
};

Connection::Connection(IConnectionCommunication* communication, ClientSideControlChannel* controlChannel, QObject *parent) :
    QObject(parent),
    _d(new ConnectionPrivate(communication, controlChannel, this))
{
}

Connection::~Connection()
{
}

bool Connection::isConnected() const
{
    return _d->controlChannel->isConnected();
}

bool Connection::isActivated() const
{
    return _d->controlChannel->isActivated();
}

bool Connection::isHeadServerConnected() const
{
    return _d->isHeadServerConnected;
}
