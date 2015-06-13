#include "commschannelfactory.h"

CommsChannelFactory::CommsChannelFactory(QSharedPointer<IApplications> apps) :
    _iapps(apps)
{

}

CommsChannelFactory::~CommsChannelFactory()
{

}

ControlChannel* CommsChannelFactory::createControlChannel(int connectionId)
{
    // TODO: from where to get app id
    ControlChannel* channel = ChannelFactory::createControlChannel(connectionId);

    if (applicationId == "mainui") {
        // channel takes ownership of command
        channel->registerCommand(new QueryLocalApplicationsCommand(iapps));
    }

    // caller takes ownership of channel
    return channel;
}
