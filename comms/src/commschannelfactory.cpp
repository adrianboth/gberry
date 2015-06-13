#include "commschannelfactory.h"

#include <QString>

#include "commands/commscommands.h"
#include <server/applicationregistry.h>
#include <server/serversidecontrolchannel.h>

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

    if (applicationId == "mainui") {
        // channel takes ownership of command
        channel->registerCommand(_commands->createQueryLocalApplicationsCommand());

        channel->registerCommand(_commands->createLaunchApplicationCommand());

        channel->registerCommand(_commands->createExitApplicationCommand(connectionId));

    }

    // caller takes ownership of channel
    return channel;
}

} // eon
