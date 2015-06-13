#ifndef TESTCLIENTSIDECHANNELPARTNERS_H
#define TESTCLIENTSIDECHANNELPARTNERS_H

#include "client/clientsidechannelpartners.h"

class TestClientSidePlayerChannelPartner : public ClientSidePlayerChannelPartner
{
public:
    explicit TestClientSidePlayerChannelPartner(int channelId) :
        ClientSidePlayerChannelPartner(channelId),
        playerMessageCount(0),
        playerExitCalledCount(0) {}
    virtual ~TestClientSidePlayerChannelPartner() {}

    int playerMessageCount;
    QByteArray lastMessage;

    int playerExitCalledCount;
    virtual void playerMessage(const QByteArray& msg) override {
        playerMessageCount++;
        lastMessage = msg;
    }

    virtual void playerExit() override {
        playerExitCalledCount++;
    }
};

class TestClientSideChannelPartner : public ClientSideChannelPartner
{
public:
    explicit TestClientSideChannelPartner(int channelId) :
        ClientSideChannelPartner(channelId),
        messageCount(0),
        closeChannelCalled(0) {}
    virtual ~TestClientSideChannelPartner() {}

    int messageCount;
    QByteArray lastMessage;

    int closeChannelCalled;

    virtual void sendMessage(const QByteArray& msg) override {
        messageCount++;
        lastMessage = msg;
    }
    virtual void closeChannel() override {
        closeChannelCalled++;
    }
};
#endif // TESTCLIENTSIDECHANNELPARTNERS_H

