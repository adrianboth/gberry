#ifndef ICLIENTSIDECHANNELPARTNER_H
#define ICLIENTSIDECHANNELPARTNER_H

class QByteArray;

class ClientSideChannelPartner
{
public:
    explicit ClientSideChannelPartner(int channelId) : _channelId(channelId) {}
    virtual ~ClientSideChannelPartner() {}
    virtual void sendMessage(const QByteArray& msg) = 0;
    virtual void closeChannel() = 0;

protected:
    int _channelId;

};

class ClientSidePlayerChannelPartner
{
public:
    explicit ClientSidePlayerChannelPartner(int channelId) : _channelId(channelId) {}
    virtual ~ClientSidePlayerChannelPartner() {}

    virtual void playerMessage(const QByteArray& msg) = 0;
    virtual void playerExit() = 0;

protected:
    int _channelId;
};

#endif // ICLIENTSIDECHANNELPARTNER_H

