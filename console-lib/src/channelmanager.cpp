#include "channelmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "playerchannel.h"


ChannelManager::ChannelManager(QObject *parent) :
    QObject(parent)
{

}

ChannelManager::~ChannelManager()
{
    qDebug("### ~ChannelManager");
}

QList<int> ChannelManager::allChannelIds()
{
    return _channels.keys();
}

void ChannelManager::registerChannel(Channel *handler)
{
    if (_channels.contains(handler->channelId()))
    {
        // TODO: raise error
        qWarning() << "Discarding handler registration with ID" << handler->channelId() << "because id already used";
        return;
    }
    _channels[handler->channelId()] = handler;
    handler->setChannelHandlerParent(this);
}

Channel* ChannelManager::unregisterChannel(int channelId)
{
    if (_channels.contains(channelId))
    {
        qDebug("### unregistering handler");
        Channel* handler = _channels[channelId];
        if (handler->state() == Channel::CHANNEL_OPEN)
            closeChannel(channelId);

        _channels.remove(channelId);
        return handler;
    }
    return NULL;
}

void ChannelManager::processMessage(int channelId, const QByteArray msg)
{
    qDebug() << "### ChannelManager::processMessage: cid=" << channelId;
    if (_channels.contains(channelId))
    {
        Channel* handler = _channels[channelId];
        // TODO: does this cause problems for testing
        // TODO: should condition be == CHANNEL_OPEN
        //if (handler->state() != ChannelHandler::CHANNEL_CLOSED)
        {
            handler->receiveMessage(msg);
        }
    }
    qDebug() << "### OUT ChannelManager::processMessage: cid=" << channelId;
}

void ChannelManager::channelSendMessage(int channelId, const QByteArray msg)
{
    emit outgoingMessage(channelId, msg);
}

void ChannelManager::closeChannel(int channelId)
{
    qDebug() << "### ChannelManager::closeChannel(): cid=" << channelId;
    // channel needs to be registered beforehand
    if (_channels.contains(channelId))
    {
        Channel* handler = _channels[channelId];
        QJsonObject json;

        json["command"] = "CloseChannel";
        QJsonDocument jsonDoc(json);
        QByteArray msg = jsonDoc.toJson();

        handler->setState(Channel::CHANNEL_CLOSED);
        emit outgoingMessage(handler->channelId(), msg);
    }
}

void ChannelManager::channelDestroyed(int channelId)
{
    // cleanup references
    unregisterChannel(channelId);
}

void ChannelManager::channelCloseReceived(int channelId)
{
    emit channelClosed(channelId);
}
