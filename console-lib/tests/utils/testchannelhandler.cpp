#include "testchannelhandler.h"

TestChannelHandler::TestChannelHandler(int channelId) :
    ChannelHandler(channelId), received(0)
{

}

TestChannelHandler::~TestChannelHandler()
{

}

void TestChannelHandler::triggerOutgoingMessage(QByteArray msg)
{
    lastSentMsg = msg;
    emit outgoingMessage(lastSentMsg);
}

void TestChannelHandler::handleMessage(const QByteArray &msg)
{
    received++;
    lastReceivedMsg = QString(msg);
}


// TODO: response id concept?

// TODO: meta data?
