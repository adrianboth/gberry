#ifndef TESTCHANNEL_H
#define TESTCHANNEL_H

#include "server/channel.h"


class TestChannel : public Channel
{
    Q_OBJECT

public:
    TestChannel(int channelId);
    ~TestChannel();

    void triggerOutgoingMessage(QByteArray msg);
    int received;
    QString lastReceivedMsg;
    QByteArray lastSentMsg;

    // Channel
    virtual void reopen() {}

public slots:
    // Channel
    bool receiveMessageFromSouth(const QByteArray msg);

};

#endif // TESTCHANNEL_H
