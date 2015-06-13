#ifndef TESTPLAYERCHANNELNORTHPARTNER_H
#define TESTPLAYERCHANNELNORTHPARTNER_H

#include "server/serversideplayerchannel.h"

class TestPlayerChannelNorthPartner : public PlayerChannelNorthPartner
{

public:
    explicit TestPlayerChannelNorthPartner(int channelId);
    virtual ~TestPlayerChannelNorthPartner();

    virtual void playerMessageFromSouth(const QByteArray& msg) override;

    int messageCount;
    QByteArray lastMessage;
};

#endif // TESTPLAYERCHANNELNORTHPARTNER_H
