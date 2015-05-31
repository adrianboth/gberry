#include "testchannel.h"

TestChannel::TestChannel(int channelId) :
    Channel(channelId), received(0)
{
}

TestChannel::~TestChannel()
{
}

void TestChannel::triggerOutgoingMessage(QByteArray msg)
{
    lastSentMsg = msg;
    sendMessage(lastSentMsg);
}

bool TestChannel::receiveMessage(const QByteArray msg)
{
    received++;
    lastReceivedMsg = QString(msg);
    return true;
}

// TODO: response id concept?
