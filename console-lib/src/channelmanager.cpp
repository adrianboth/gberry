#include "channelmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "playercontrolchannel.h"


ChannelManager::ChannelManager(QObject *parent) :
    QObject(parent)
{

}

ChannelManager::~ChannelManager()
{
    qDebug("### ~ChannelManager");
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

    connect(handler, &ChannelHandler::closeReceived,
            this,    &ChannelManager::channelCloseReceived);

    // This is just backup measure to prevent segfaults. (e.g. during failing tests)
    // (we can't use destroyed() as we need to handle things ...) (TODO: did I get semantics of destroyed) wrong ... but there was segfault
    connect(handler, &ChannelHandler::closing,
            [this] (ChannelHandler* handler) {
        qDebug() << "### closing: channelId=" << handler->channelId();
        this->unregisterHandler(handler->channelId());
    });

}

ChannelHandler* ChannelManager::unregisterHandler(int channelId)
{
    if (_handlers.contains(channelId))
    {
        qDebug("### unregistering handler");
        ChannelHandler* handler = _handlers[channelId];
        if (handler->state() == ChannelHandler::CHANNEL_OPEN)
            closeChannel(channelId);

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
        QJsonDocument doc(QJsonDocument::fromJson(msg));
        QJsonObject json(doc.object());
        // TODO: common impl for json validation
        if (json.contains("command") && json["command"] == "OpenChannel")
        {
            // TODO: handle metadata

            // note controls created by ChannelManager will be deleted when
            // ChannelManager is destroyed
            PlayerControlChannel* playerControl = new PlayerControlChannel(channelId, this);
            registerHandler(playerControl);
            playerControl->setState(ChannelHandler::CHANNEL_OPEN);
            emit newChannel(channelId);

            QJsonObject json;
            json["command"] = "OpenChannelAccepted"; // TODO: how denial would go, we want need to make query instead of signal (above)
            QJsonDocument jsonDoc(json);
            QByteArray msg = jsonDoc.toJson();

            emit outgoingMessage(channelId, msg);
        }
        else
        {
            qWarning() << "Message for unhandled channel, id =" << channelId;
        }
        return;
    }

    ChannelHandler* handler = _handlers[channelId];
    // TODO: does this cause problems for testing
    // TODO: should condition be == CHANNEL_OPEN
    //if (handler->state() != ChannelHandler::CHANNEL_CLOSED)
    {
        handler->handleMessage(msg);
    }
}

void ChannelManager::outgoingMessageFromChannelHandler(const QByteArray &msg)
{
    if (sender())
    {
        ChannelHandler* handler = qobject_cast<ChannelHandler*>(sender());
        emit outgoingMessage(handler->channelId(), msg);
    }
}


void ChannelManager::openChannel(int channelId)
{
    // channel needs to be registered beforehand
    if (_handlers.contains(channelId))
    {
        ChannelHandler* handler = _handlers[channelId];
        QJsonObject json;

        json["command"] = "OpenChannel";
        //json["metadata"] = handler->getMetadataJson();

        QJsonDocument jsonDoc(json);
        QByteArray msg = jsonDoc.toJson();

        handler->setState(ChannelHandler::CHANNEL_OPEN_ONGOING);
        emit outgoingMessage(handler->channelId(), msg);
    }
}

void ChannelManager::closeChannel(int channelId)
{
    // channel needs to be registered beforehand
    if (_handlers.contains(channelId))
    {
        ChannelHandler* handler = _handlers[channelId];
        QJsonObject json;

        json["command"] = "CloseChannel";
        QJsonDocument jsonDoc(json);
        QByteArray msg = jsonDoc.toJson();

        handler->setState(ChannelHandler::CHANNEL_CLOSED);
        emit outgoingMessage(handler->channelId(), msg);
    }
}

void ChannelManager::channelCloseReceived(ChannelHandler *handler)
{
    // do not send any message
    int channelId = handler->channelId();
    unregisterHandler(channelId);

    emit channelClosed(channelId);
}
