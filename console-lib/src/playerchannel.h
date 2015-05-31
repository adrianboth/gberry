#ifndef PLAYERCONTROLCHANNEL_H
#define PLAYERCONTROLCHANNEL_H

#include "channel.h"


class PlayerChannel : public Channel
{
    Q_OBJECT

public:
    PlayerChannel(int channelId, IChannelParent* channelParent, QObject* parent = 0);
    ~PlayerChannel();

    virtual void sendPlayerMessage(QByteArray msg);

    // Channel
    virtual bool receiveMessage(const QByteArray msg);
    virtual void reopen() {} // dummy impl

signals:
    void playerMessageReceived(int channelId, const QByteArray msg);

};

#endif // PLAYERCONTROLCHANNEL_H
