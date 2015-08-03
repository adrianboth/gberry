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
 
#include "commscommands.h"

#include <server/applicationregistry.h>
#include <server/serversidecontrolchannel.h>
#include <server/playersessionmanager.h>

#include "commands/launchapplicationcommand.h"
#include "commands/exitapplicationcommand.h"
#include "commands/querylocalapplicationscommand.h"
#include "commands/querydownloadableapplicationscommand.h"
#include "commands/downloadapplicationcommand.h"
#include "commands/headserverstatuscommand.h"
#include "localapplicationsstorage.h"

using namespace GBerry;

namespace GBerryComms {

CommsCommands::CommsCommands(LocalApplicationsStorage* applicationsStorage,
                             ApplicationRegistry* registry,
                             HeadServerConnection* headServerConnection,
                             DownloadableApplicationCache* downloadableApplicationCache,
                             PlayerSessionManager* playerSessions,
                             QObject *parent) :
    QObject(parent),
    _applicationRegistry(registry),
    _headServerConnection(headServerConnection),
    _downloadableApplicationCache(downloadableApplicationCache),
    _applicationsStorage(applicationsStorage),
    _playerSessions(playerSessions)
{
    QSharedPointer<LocalApplications> apps(new LocalApplications(_applicationsStorage));
    _iapps = (qSharedPointerCast<IApplications>(apps));
}

CommsCommands::~CommsCommands()
{
}

ICommand* CommsCommands::createLaunchApplicationCommand()
{
    LaunchApplicationCommand* cmd = new LaunchApplicationCommand(_iapps);
    connect(cmd, &LaunchApplicationCommand::launchApplicationRequested,
            [&] (QString applicationId) { emit this->launchApplicationRequested(applicationId); });

    return cmd;
}

ICommand *CommsCommands::createExitApplicationCommand(int connectionId)
{
    QString applicationId = _applicationRegistry->applicationIdByConnectionId(connectionId);

    ExitApplicationCommand* cmd = new ExitApplicationCommand(applicationId);
    connect(cmd, &ExitApplicationCommand::exitApplicationRequested,
            [&] (QString appId) { emit this->exitApplicationRequested(appId); });

    return cmd;
}

ICommand* CommsCommands::createQueryLocalApplicationsCommand()
{
    return new QueryLocalApplicationsCommand(_iapps);
}

ICommand* CommsCommands::createQueryDownloadableApplicationsCommand(
        ServerSideControlChannel* controlChannel)
{
    return new QueryDownloadableApplicationsCommand(
                _headServerConnection,
                controlChannel,
                _downloadableApplicationCache,
                _playerSessions);
}

ICommand *CommsCommands::createDownloadApplicationCommand(ServerSideControlChannel *controlChannel)
{
    return new DownloadApplicationCommand(
                _headServerConnection,
                controlChannel,
                _downloadableApplicationCache,
                _applicationsStorage,
                _playerSessions);
}

ICommand *CommsCommands::createHeadServerStatusCommand(ServerSideControlChannel *controlChannel)
{
    return new HeadServerStatusCommand(
                _headServerConnection,
                controlChannel);
}

} // eon
