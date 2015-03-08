#include "channel.h"

#define LOG_AREA "Channel"
#include "log/log.h"

Channel::Channel(
        int channelId,
        IChannelParent* channelParent,
        QObject* parent) :
    QObject(parent), _id(channelId), _state(CHANNEL_CLOSED), _channelParent(channelParent)
{
}

Channel::~Channel()
{
    TRACE("~Channel");
    if (_channelParent)
        _channelParent->channelDestroyed(_id);
}

void Channel::setChannelHandlerParent(IChannelParent *channelParent)
{
    _channelParent = channelParent;
}

void Channel::detachChannelHandlerParent()
{
    _channelParent = NULL;
}

int Channel::channelId()
{
    return _id;
}

Channel::ChannelState Channel::state()
{
    return _state;
}

void Channel::setState(ChannelState newState)
{
    _state = newState;
}

void Channel::sendMessage(const QByteArray msg)
{
    if (_channelParent)
    {
        _channelParent->channelSendMessage(_id, msg);
    }
}

void Channel::closeReceived()
{
    setState(Channel::CHANNEL_CLOSED);
    if (_channelParent)
    {
        _channelParent->channelCloseReceived(_id);
    }
    emit channelClosed();
}

bool Channel::isOpen()
{
    return state() == Channel::CHANNEL_OPEN;
}

void Channel::close()
{
    setState(Channel::CHANNEL_CLOSED);
    emit channelClosed();
}
