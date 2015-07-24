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
 
 #include "connectioncommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "ConnectionCommunication"
#include "log/log.h"


class HeadServerStatusCommand : public ICommand
{
public:
    HeadServerStatusCommand(ConnectionCommunication* comm) :
        ICommand("HeadServerStatus"),
        _comm(comm) {}

    virtual ~HeadServerStatusCommand() {}

    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): HeadServerStatusCommand");
        // parsing will occur in Connection

        // correct signal emit will require right interface
        IConnectionCommunication* icomm = qobject_cast<IConnectionCommunication*>(_comm);
        emit icomm->messageReceived(json);
        return true;
    }

private:
    ConnectionCommunication* _comm;
};



ConnectionCommunication::ConnectionCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IConnectionCommunication(parent),
    _controlChannel(controlChannel)
{
    controlChannel->registerCommand(new HeadServerStatusCommand(this)); // takes ownership
}

ConnectionCommunication::~ConnectionCommunication()
{
}

void ConnectionCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}
