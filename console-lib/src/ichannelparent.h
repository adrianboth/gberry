#ifndef ICHANNELPARENT_H
#define ICHANNELPARENT_H

class IChannelParent
{
public:

    virtual void channelDestroyed(int channelId) = 0;

    virtual void channelCloseReceived(int channelId) = 0;

    virtual void channelSendMessage(int channelId, const QByteArray msg) = 0; // TODO: remove ref

};

#endif // ICHANNELPARENT_H

