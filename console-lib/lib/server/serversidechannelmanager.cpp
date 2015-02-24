#include "serversidechannelmanager.h"

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
    foreach(auto handler, _handlers.values())
    {
        handler->setState(ChannelHandler::CHANNEL_CLOSED);
    }
}
