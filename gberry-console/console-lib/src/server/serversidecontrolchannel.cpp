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
 
 #include "serversidecontrolchannel.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "common/messagefactory.h"

using namespace GBerry::Console;

#define LOG_AREA "ServerSideControlChannel"
#include "log/log.h"


const int ServerSideControlChannel::CHANNEL_ID = 0;


ServerSideControlChannel::ServerSideControlChannel() :
    Channel(CHANNEL_ID)
{
}

ServerSideControlChannel::~ServerSideControlChannel()
{
    TRACE("~ServerSideControlChannel");
}

void ServerSideControlChannel::pingSouth()
{
    DEBUG("Ping south requested");
    sendMessageToSouth(MessageFactory::createPingCommand());
}
