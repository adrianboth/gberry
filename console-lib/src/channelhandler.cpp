#include "channelhandler.h"


ChannelHandler::ChannelHandler(int channelId, QObject* parent) :
    QObject(parent), _id(channelId), _state(CHANNEL_CLOSED)
{
}

int ChannelHandler::channelId()
{
    return _id;
}

ChannelHandler::ChannelState ChannelHandler::state()
{
    return _state;
}

void ChannelHandler::setState(ChannelState newState)
{
    _state = newState;
}
