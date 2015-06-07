#include "channelmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "playerchannel.h"

#define LOG_AREA "ChannelManager"
#include "log/log.h"

ChannelManager::ChannelManager(QObject *parent) :
    QObject(parent)
{

}

ChannelManager::~ChannelManager()
{
    TRACE("~ChannelManager");
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
        TRACE("Unregistering handler: cid=" << channelId);
        Channel* handler = _channels[channelId];
        if (handler->state() == Channel::CHANNEL_OPEN)
            closeChannel(channelId);

        _channels.remove(channelId);
        handler->detachChannelHandlerParent(); // no more callbacks
        return handler;
    }
    return NULL;
}

void ChannelManager::processMessage(int channelId, const QByteArray msg)
{
    TRACE("processMessage(): cid=" << channelId << ", data=" << msg);

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

void ChannelManager::registerCommand(QSharedPointer<ICommand> cmd)
{
    if (_commands.contains(cmd->id())) {
        WARN("Command" << cmd->id() << "already registered, ignoring new regisration");
        return;
    }
}

class CommandResponseImpl : public ICommandResponse
{
public:
    virtual void set(QJsonObject j) override { json = j; }
    QJsonObject json;
};

bool ChannelManager::processCommand(int channelId, const QJsonObject &json)
{
    if (!json.contains("command"))
        return false;

    QString cmd = json["command"].toString();
    if (!_commands.contains(cmd))
        return false;

    CommandResponseImpl response;
    bool result = _commands[cmd]->process(json, response);
    if (!response.json.empty()) {
        // TODO: should we automatically add "command_response" ?
        QJsonDocument jsonDoc(response.json);
        emit outgoingMessage(channelId, jsonDoc.toJson());
    }

    return result;
}
