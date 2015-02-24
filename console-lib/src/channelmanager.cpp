#include "channelmanager.h"

#include <QDebug>


ChannelManager::ChannelManager(QObject *parent) :
    QObject(parent)
{

}

ChannelManager::~ChannelManager()
{

}

void ChannelManager::registerHandler(ChannelHandler *handler)
{
    if (_handlers.contains(handler->channelId()))
    {
        // TODO: raise error
        qWarning() << "Discarding handler registration with ID" << handler->channelId() << "because id already used";
        return;
    }
    _handlers[handler->channelId()] = handler;

    connect(handler, &ChannelHandler::outgoingMessage,
            this,    &ChannelManager::outgoingMessageFromChannelHandler);
}

ChannelHandler* ChannelManager::unregisterHandler(int channelId)
{
    if (_handlers.contains(channelId))
    {
        ChannelHandler* handler = _handlers[channelId];
        _handlers.remove(channelId);

        disconnect(handler, &ChannelHandler::outgoingMessage,
                   this,    &ChannelManager::outgoingMessageFromChannelHandler);

        return handler;
    }

    return NULL;
}

void ChannelManager::processMessage(int channelId, const QByteArray &msg)
{
    if (!_handlers.contains(channelId))
    {
        qWarning() << "Message for unhandled channel, id =" << channelId;
        return;
    }

    ChannelHandler* handler = _handlers[channelId];
    handler->handleMessage(msg);
}

void ChannelManager::outgoingMessageFromChannelHandler(const QByteArray &msg)
{
    if (sender())
    {
        ChannelHandler* handler = qobject_cast<ChannelHandler*>(sender());
        emit outgoingMessage(handler->channelId(), msg);
    }
}
