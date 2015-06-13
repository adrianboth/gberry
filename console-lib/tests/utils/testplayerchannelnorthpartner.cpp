#include "testplayerchannelnorthpartner.h"

TestPlayerChannelNorthPartner::TestPlayerChannelNorthPartner(int channelId) :
    PlayerChannelNorthPartner(channelId),
    messageCount(0)
{
}

TestPlayerChannelNorthPartner::~TestPlayerChannelNorthPartner()
{
}

void TestPlayerChannelNorthPartner::playerMessageFromSouth(const QByteArray& msg)
{
    messageCount++;
    lastMessage = msg;
}
