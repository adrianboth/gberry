#include "commscommands.h"

#include <server/applicationregistry.h>
#include <server/serversidecontrolchannel.h>

#include "commands/launchapplicationcommand.h"
#include "commands/exitapplicationcommand.h"
#include "commands/querylocalapplicationscommand.h"
#include "commands/querydownloadableapplicationscommand.h"
#include "commands/downloadapplicationcommand.h"
#include "localapplicationsstorage.h"

using namespace GBerry;

namespace GBerryComms {

CommsCommands::CommsCommands(LocalApplicationsStorage* applicationsStorage,
                             ApplicationRegistry* registry,
                             HeadServerConnection* headServerConnection,
                             DownloadableApplicationCache* downloadableApplicationCache,
                             QObject *parent) :
    QObject(parent),
    _applicationRegistry(registry),
    _headServerConnection(headServerConnection),
    _downloadableApplicationCache(downloadableApplicationCache),
    _applicationsStorage(applicationsStorage)
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
                _downloadableApplicationCache);
}

ICommand *CommsCommands::createDownloadApplicationCommand(ServerSideControlChannel *controlChannel)
{
    return new DownloadApplicationCommand(
                _headServerConnection,
                controlChannel,
                _downloadableApplicationCache,
                _applicationsStorage);
}

} // eon
