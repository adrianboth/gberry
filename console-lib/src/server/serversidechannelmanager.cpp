#include "serversidechannelmanager.h"
#include "serversideplayerchannel.h"


ServerSideChannelManager::ServerSideChannelManager(QObject* parent) :
    ChannelManager(parent), _channelIdCount(1)
{

}

ServerSideChannelManager::~ServerSideChannelManager()
{

}

int ServerSideChannelManager::nextFreeChannelId()
{
    // NOTE: Now just increasing number, at some point
    //        it would overflow but as it is 32bit number it is not
    //        likely in normal use.
    return _channelIdCount++;
}

void ServerSideChannelManager::applicationClosed()
{
    foreach(auto channel, _channels.values())
    {
        channel->close();
    }
}

PlayerChannel* ServerSideChannelManager::openPlayerChannel(PlayerMetadata meta)
{
    ServerSidePlayerChannel* channel = new ServerSidePlayerChannel(nextFreeChannelId(), meta);
    registerChannel(channel);
    channel->open();
    return channel;
}

void ServerSideChannelManager::reopenPlayerChannels()
{
    foreach(int cid, allChannelIds())
    {
        // currently we assume all other than root are player channels
        if (cid != ChannelManager::ROOT_CHANNEL)
        {
            _channels[cid]->reopen();
        }
    }
}
