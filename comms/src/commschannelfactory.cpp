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

        channel->registerCommand(_commands->createLaunchApplicationCommand());   

    } else {
        channel->registerCommand(_commands->createExitApplicationCommand(connectionId));
    }

    // caller takes ownership of channel
    return channel;
}

} // eon
