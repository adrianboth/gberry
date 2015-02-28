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

void TestChannel::receiveMessage(const QByteArray msg)
{
    received++;
    lastReceivedMsg = QString(msg);
}

// TODO: response id concept?
