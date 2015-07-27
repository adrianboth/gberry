#ifndef STUB_CHANNELSOUTHPARTNER_H
#define STUB_CHANNELSOUTHPARTNER_H

#include <QList>
#include <QByteArray>

#include "server/channel.h"

namespace GBerryConsole {
namespace Test {

class StubChannelSouthPartner : public ChannelSouthPartner
{
public:
    StubChannelSouthPartner() {}
    virtual ~StubChannelSouthPartner() {}

    // channel has received closing message from other end
    virtual void channelCloseReceived() override { Q_ASSERT(false); } // should not happen in this tes

    int channelSendMessageToSouthCallCount{0};
    QList<QByteArray> receivedMessages;

    QByteArray lastSentMessage;
    virtual void channelSendMessageToSouth(const QByteArray& msg) override {
        channelSendMessageToSouthCallCount++;
        receivedMessages.append(msg);
    }

};

}} // eon

#endif // STUB_CHANNELSOUTHPARTNER_H
