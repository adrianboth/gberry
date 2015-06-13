#include "channelfactory.h"

#include "serversideplayerchannel.h"
#include "serversidecontrolchannel.h"
#include "server/commands/pingcommand.h"

namespace GBerry {
namespace Console {
namespace Server {

ChannelFactory::ChannelFactory()
{
}

ChannelFactory::~ChannelFactory()
{
}

ServerSidePlayerChannel* ChannelFactory::createPlayerChannel(int channelId, PlayerMeta meta)
{
    ServerSidePlayerChannel* channel = new ServerSidePlayerChannel(channelId, meta);
    return channel;
}

ServerSideControlChannel* ChannelFactory::createControlChannel(int connectionId)
{
    Q_UNUSED(connectionId);
    ServerSideControlChannel* channel = new ServerSideControlChannel();
    channel->registerCommand(new PingCommand(channel));
    channel->registerCommand(new PingReply(channel));

    return channel;
    // TODO: create generic setup
    // TODO: this function should be inherited to decorate (add things)
}

}}} // eon
