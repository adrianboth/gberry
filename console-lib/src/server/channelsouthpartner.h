#ifndef CHANNELSOUTHPARTNER_H
#define CHANNELSOUTHPARTNER_H

class QByteArray;

class ChannelSouthPartner
{
public:
    ChannelSouthPartner() {}
    virtual ~ChannelSouthPartner() {}

    // channel has received closing message from other end
    virtual void channelCloseReceived() = 0;

    virtual void channelSendMessageToSouth(const QByteArray& msg) = 0;

};

#endif // CHANNELSOUTHPARTNER_H

