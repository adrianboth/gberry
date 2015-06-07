#ifndef ICHANNELPARENT_H
#define ICHANNELPARENT_H

#include <QByteArray>
#include <QJsonObject>

class IChannelParent

{
public:

    virtual void channelDestroyed(int channelId) = 0;

    virtual void channelCloseReceived(int channelId) = 0;

    virtual void channelSendMessage(int channelId, const QByteArray msg) = 0; // TODO: remove ref

    virtual bool processCommand(int channelId, const QJsonObject& json) = 0;
};

#endif // ICHANNELPARENT_H

