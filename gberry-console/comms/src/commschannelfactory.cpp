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
 
 #include "commschannelfactory.h"

#include <QString>

#include "commands/commscommands.h"
#include <server/applicationregistry.h>
#include <server/serversidecontrolchannel.h>

#define LOG_AREA "CommsChannelFactory"
#include "log/log.h"

namespace GBerryComms {

CommsChannelFactory::CommsChannelFactory(ApplicationRegistry* registry, CommsCommands* commands) :
    _registry(registry),
    _commands(commands)
{
}

CommsChannelFactory::~CommsChannelFactory()
{
}

ServerSideControlChannel* CommsChannelFactory::createControlChannel(int connectionId)
{
    ServerSideControlChannel* channel = ChannelFactory::createControlChannel(connectionId);
    QString applicationId = _registry->applicationIdByConnectionId(connectionId);

    DEBUG("Creating control channel: connectionId =" << connectionId << ", applicationId =" << applicationId);

    if (applicationId == "mainui") {
        // channel takes ownership of command
        channel->registerCommand(_commands->createQueryLocalApplicationsCommand());

        channel->registerCommand(_commands->createQueryDownloadableApplicationsCommand(channel));

        channel->registerCommand(_commands->createDownloadApplicationCommand(channel));

        channel->registerCommand(_commands->createLaunchApplicationCommand());

        channel->registerCommand(_commands->createExitConsoleCommand());

    } else {    
        channel->registerCommand(_commands->createExitApplicationCommand(connectionId));
    }

    // TODO: does it make sense to hav this for all
    channel->registerCommand(_commands->createHeadServerStatusCommand(channel));

    // caller takes ownership of channel
    return channel;
}

} // eon
