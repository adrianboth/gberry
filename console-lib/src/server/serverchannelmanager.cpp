#include "serverchannelmanager.h"

#include <QJsonDocument>

#include "serversidecontrolchannel.h"
#include "serversideplayerchannel.h"
#include "serversideplayerchannel.h"
#include "serversidecontrolchannel.h"
#include "channelfactory.h"

#define LOG_AREA "ServerChannelManager"
#include "log/log.h"

class ServerChannelManagerPrivate
{
public:
    ServerChannelManagerPrivate(ChannelFactory* factory, ServerChannelManager* q_) :
        channelIdCount(1),
        channelFactory(factory),
        q(q_),
        activeConnectionId(-1) {}

    ~ServerChannelManagerPrivate() {
        foreach(auto playerChannel, playerChannels.values()) {
            delete playerChannel;
        }
        foreach(auto controlChannel, controlChannels.values()) {
            delete controlChannel;
        }
    }

    int channelIdCount;
    // destroyed outside
    ChannelFactory* channelFactory;
    ServerChannelManager* q;

    QMap<int, ServerSidePlayerChannel*> playerChannels; // by channelId
    QMap<int, ServerSideControlChannel*> controlChannels; // by connectionId
    QList<int> activeConnectionIdsStack;
    int activeConnectionId;

    /* */
    bool hasActiveConnection() const { return activeConnectionId != -1; }

    /* */
    void reopenPlayerChannels() {
        DEBUG("Reopening all player channels");
        foreach(auto playerChannel, playerChannels.values()) {
            playerChannel->open();
        }
    }

    // Sends message informing client that channel is about to be closed.
    // Typically for PlayerChannels but works also for ControlChannels
    //
    void sendCloseChannelMessageToSouth(int connectionId, Channel* channel)
    {
        if (!channel) {
            ERROR("Invalid channel handled (NULL)");
            return;
        }

        DEBUG("Closing channel: cid=" << channel->channelId());
        QJsonObject json;
        json["command"] = "CloseChannel";
        QJsonDocument jsonDoc(json);
        QByteArray msg = jsonDoc.toJson();

        channel->close();
        emit q->outgoingMessageToSouth(connectionId, channel->channelId(), msg);
    }

};


class ControlChannelSouthPartnerImpl : public ChannelSouthPartner
{
public:
    ControlChannelSouthPartnerImpl(int connectionId_, ServerChannelManager* manager_) :
        connectionId(connectionId_),
        manager(manager_) {}

    int connectionId;
    ServerChannelManager* manager;

    virtual void channelCloseReceived() override
    {
    }
    virtual void channelSendMessageToSouth(const QByteArray& msg) override
    {
        emit manager->outgoingMessageToSouth(connectionId, ServerSideControlChannel::CHANNEL_ID, msg);
    }
};

class PlayerChannelSouthPartnerImpl : public ChannelSouthPartner
{
public:
    PlayerChannelSouthPartnerImpl(int channelId_, ServerChannelManager* manager_) :
        channelId(channelId_),
        manager(manager_) {}

    int channelId;
    ServerChannelManager* manager;

    virtual void channelCloseReceived() override
    {
        //emit manager->channelClosed(channelId);
    }
    virtual void channelSendMessageToSouth(const QByteArray& msg) override
    {
        if (manager->_d->hasActiveConnection())
            emit manager->outgoingMessageToSouth(manager->_d->activeConnectionId, channelId, msg);
    }
};

ServerChannelManager::ServerChannelManager(ChannelFactory* factory, QObject* parent) :
    QObject(parent),
    _d(new ServerChannelManagerPrivate(factory, this))
{
}

ServerChannelManager::~ServerChannelManager()
{
}

int ServerChannelManager::nextFreeChannelId()
{
    // NOTE: Now just increasing number, at some point
    //        it would overflow but as it is 32bit number it is not
    //        likely in normal use.
    return _d->channelIdCount++;
}

void ServerChannelManager::applicationClosed(int connectionId)
{
    if (!_d->controlChannels.contains(connectionId)) {
        WARN("Unknown application closed: connectionId =" << connectionId);
        return;
    }

    ServerSideControlChannel* ch = _d->controlChannels[connectionId];
    _d->controlChannels.remove(connectionId);
    ch->close();
    ch->detachSouthPartner();
    ch->deleteLater();

    foreach(auto playerChannel, _d->playerChannels.values()) {
        // this is like pausing, do not delete, they can be reopened
        playerChannel->close();
    }

    if (connectionId == _d->activeConnectionId) {
        _d->activeConnectionIdsStack.pop_front(); // this was the active
        if (!_d->activeConnectionIdsStack.isEmpty()) {
            _d->activeConnectionId = _d->activeConnectionIdsStack.front();
            _d->reopenPlayerChannels();
        } else {
            _d->activeConnectionId = -1;
        }
    }
}

ServerSidePlayerChannel* ServerChannelManager::openPlayerChannel(PlayerMeta meta)
{
    DEBUG("Open new player channel: pid=" << meta.playerId);
    int channelId = nextFreeChannelId();
    ServerSidePlayerChannel* channel = _d->channelFactory->createPlayerChannel(channelId, meta);

    _d->playerChannels[channel->channelId()] = channel;
    channel->attachSouthPartner(new PlayerChannelSouthPartnerImpl(channelId, this));

    channel->open();
    return channel;
}

ServerSideControlChannel* ServerChannelManager::openControlChannel(int connectionId)
{
    ServerSideControlChannel* channel = _d->channelFactory->createControlChannel(connectionId);

    channel->attachSouthPartner(new ControlChannelSouthPartnerImpl(connectionId, this));
    _d->controlChannels[connectionId] = channel;
    channel->open();

    // when control channel is created, connection has been already established
    return channel;
}

QList<int> ServerChannelManager::allPlayerChannelIds()
{
    return _d->playerChannels.keys();
}

void ServerChannelManager::closePlayerChannel(int channelId)
{
    if (_d->playerChannels.contains(channelId))
    {
        TRACE("Closing PlayerChannel: cid=" << channelId);
        Channel* ch = _d->playerChannels[channelId];
        if (ch->isOpen())
            _d->sendCloseChannelMessageToSouth(_d->activeConnectionId, ch);

        _d->playerChannels.remove(channelId);
        ch->detachSouthPartner(); // no more callbacks
        ch->deleteLater();
    }
}

void ServerChannelManager::closeControlChannel(int connectionId)
{
    if (_d->controlChannels.contains(connectionId))
    {
        TRACE("Closing ControlChannel: id=" << connectionId);
        Channel* ch = _d->controlChannels[connectionId];
        if (ch->isOpen())
            _d->sendCloseChannelMessageToSouth(connectionId, ch);

        _d->controlChannels.remove(connectionId);
        _d->activeConnectionIdsStack.removeAll(connectionId);
        if (connectionId == _d->activeConnectionId) {
            if (!_d->activeConnectionIdsStack.isEmpty()) {
                _d->activeConnectionId = _d->activeConnectionIdsStack.front();
            } else {
                _d->activeConnectionId = -1;
            }
        }
        ch->detachSouthPartner(); // no more callbacks
        ch->deleteLater();
    }
}

void ServerChannelManager::processMessageFromSouth(int connectionId, int channelId, const QByteArray& msg)
{
    TRACE("processMessage(): connectionId=" << connectionId << "cid=" << channelId << ", data=" << msg);

    if (channelId == ServerSideControlChannel::CHANNEL_ID) {
        if (_d->controlChannels.contains(connectionId)) {
            _d->controlChannels[connectionId]->receiveMessageFromSouth(msg);
        } else {
            WARN("Message from unknown connection: connectionId =" << connectionId);
        }

    // player channel
    } else if (connectionId == _d->activeConnectionId) {

        if (_d->playerChannels.contains(channelId))
        {
            Channel* handler = _d->playerChannels[channelId];
            handler->receiveMessageFromSouth(msg);

        } else {
            WARN("Message to unknown PlayerChannel");
        }
    } else {
        WARN("Player message from non-active connection. Discarded.");
    }
}

void ServerChannelManager::activateConnection(int connectionId)
{
    if (!_d->controlChannels.contains(connectionId)) {
        ERROR("Can't activate connection as it doesn't have ControlChannel registered: connectionId =" << connectionId);
        return;
    }

    if (_d->activeConnectionId != -1) {
        QJsonObject json;
        json["command"] = "Deactivate";

        emit outgoingMessageToSouth(_d->activeConnectionId,
                                    ServerSideControlChannel::CHANNEL_ID,
                                    QJsonDocument(json).toJson());
    }
    _d->activeConnectionId = connectionId;
    _d->activeConnectionIdsStack.push_front(connectionId);

    QJsonObject json;
    json["command"] = "Activate";
    emit outgoingMessageToSouth(_d->activeConnectionId,
                                ServerSideControlChannel::CHANNEL_ID,
                                QJsonDocument(json).toJson());

    _d->reopenPlayerChannels();
}

bool ServerChannelManager::hasActiveConnection()
{
    return _d->activeConnectionId != -1;
}
