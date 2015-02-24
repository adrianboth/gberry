#ifndef PLAYERCONTROLCHANNEL_H
#define PLAYERCONTROLCHANNEL_H

#include "channelhandler.h"


class PlayerControlChannel : public ChannelHandler
{
public:
    PlayerControlChannel(int channelId, QObject* parent = 0);
    ~PlayerControlChannel();

    virtual void handleMessage(const QByteArray& msg);
};

#endif // PLAYERCONTROLCHANNEL_H
