#include "testchannelfactory.h"

#include "server/serversidecontrolchannel.h"

TestChannelFactory::TestChannelFactory() :
    lastChannelId(-1),
    lastPlayerChannel(nullptr),
    lastConnectionId(-1),
    lastControlChannel(nullptr),
    controlChannelClosedCount(0)
{

}

TestChannelFactory::~TestChannelFactory()
{

}

ServerSidePlayerChannel* TestChannelFactory::createPlayerChannel(int channelId, PlayerMeta meta)
{
    lastChannelId = channelId;
    lastPlayerMeta = meta;
    lastPlayerChannel = ChannelFactory::createPlayerChannel(channelId, meta);
    return lastPlayerChannel;
}

ServerSideControlChannel* TestChannelFactory::createControlChannel(int connectionId)
{
    lastConnectionId = connectionId;
    lastControlChannel = ChannelFactory::createControlChannel(connectionId);
    connect(lastControlChannel, &ServerSideControlChannel::channelClosed,
            [&] () { controlChannelClosedCount; this->lastControlChannel = nullptr; });

    return lastControlChannel;
}
