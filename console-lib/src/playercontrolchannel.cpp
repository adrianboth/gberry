#include "playercontrolchannel.h"

PlayerControlChannel::PlayerControlChannel(int channelId, QObject* parent) :
    ChannelHandler(channelId, parent)
{

}

PlayerControlChannel::~PlayerControlChannel()
{

}

void PlayerControlChannel::handleMessage(const QByteArray &msg)
{

}
