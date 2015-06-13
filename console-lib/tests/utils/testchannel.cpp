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
    sendMessageToSouth(lastSentMsg);
}

bool TestChannel::receiveMessageFromSouth(const QByteArray msg)
{
    received++;
    lastReceivedMsg = QString(msg);
    return true;
}

// TODO: response id concept?
