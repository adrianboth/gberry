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
 
 #include "pingcommand.h"

#include "server/serversidecontrolchannel.h"
#include "common/messagefactory.h"

using namespace GBerry::Console;

namespace GBerry {
namespace Console {
namespace Server {

namespace {
    static const char* PING_ID = "Ping";
}

PingCommand::PingCommand(ServerSideControlChannel* manager) :
    ICommand(PING_ID),
    _manager(manager)
{
    Q_ASSERT(manager);
}

PingCommand::~PingCommand()
{
}

bool PingCommand::process(const QJsonObject& json, ICommandResponse& response)
{
    if (json.contains("command") && json["command"] == PING_ID)
    {
        emit _manager->pingSouthReceived();
        response.set(MessageFactory::createPingReplyJson());
        return true;
    }

    return false;
}

// ---
namespace {
    static const char* PINGREPLY_ID = "PingReply";
}

PingReply::PingReply(ServerSideControlChannel* manager) :
    ICommand(PINGREPLY_ID),
    _manager(manager)
{
    Q_ASSERT(manager);
}

PingReply::~PingReply()
{
}

bool PingReply::process(const QJsonObject& json, ICommandResponse& response)
{
    Q_UNUSED(response);
    if (json.contains("command") && json["command"] == PINGREPLY_ID)
    {
        emit _manager->pingSouthReceived();
        // but no reply to reply ...
        return true;
    }

    return false;
}

}}} //eon
