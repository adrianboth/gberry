#include "commscommands.h"

#include <server/applicationregistry.h>

#include "commands/launchapplicationcommand.h"
#include "commands/exitapplicationcommand.h"
#include "commands/querylocalapplicationscommand.h"

namespace GBerryComms {

CommsCommands::CommsCommands(QSharedPointer<IApplications> iapps,
                             ApplicationRegistry* registry,
                             QObject *parent) :
    QObject(parent),
    _iapps(iapps),
    _applicationRegistry(registry)

{
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

} // eon
