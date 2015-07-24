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
 
 #include "headserverstatuscommand.h"

#include "server/serversidecontrolchannel.h"

namespace GBerryComms {

HeadServerStatusCommand::HeadServerStatusCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel):
    ICommand("QueryHeadServerStatus"),
    _headServerConnection(headServerConnection),
    _controlChannel(controlChannel)
{
    connect(headServerConnection, &HeadServerConnection::connected,
            this, &HeadServerStatusCommand::onConnected);

    connect(headServerConnection, &HeadServerConnection::disconnected,
            this, &HeadServerStatusCommand::onDisconnected);
}

HeadServerStatusCommand::~HeadServerStatusCommand()
{
}

bool HeadServerStatusCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    Q_UNUSED(json);

    // same as there is no change, just query
    response.set(createResponse(
                     _headServerConnection->isConnected(),
                     _headServerConnection->isConnected()));
    return true;
}

void HeadServerStatusCommand::onConnected()
{
    _controlChannel->sendJsonMessageToSouth(createResponse(true, false));
}

void HeadServerStatusCommand::onDisconnected()
{
    _controlChannel->sendJsonMessageToSouth(createResponse(false, true));
}

QJsonObject HeadServerStatusCommand::createResponse(bool currentStatus, bool oldStatus)
{
    QJsonObject responseJson;
    responseJson["command"] = "HeadServerStatus"; // TODO: doesn't match to general message formats
    responseJson["status"] =  currentStatus ? "connected" : "disconnected";
    responseJson["old_status"] = oldStatus ? "connected" : "disconnected";

    return responseJson;
}

} // eon
