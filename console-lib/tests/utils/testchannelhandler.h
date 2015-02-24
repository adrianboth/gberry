#ifndef TESTCHANNELHANDLER_H
#define TESTCHANNELHANDLER_H

#include "channelhandler.h"


class TestChannelHandler : public ChannelHandler
{
    Q_OBJECT

public:
    TestChannelHandler(int channelId);
    ~TestChannelHandler();

    void triggerOutgoingMessage(QByteArray msg);
    int received;
    QString lastReceivedMsg;
    QByteArray lastSentMsg;


public slots:
    void handleMessage(const QByteArray& msg);

};

#endif // TESTCHANNELHANDLER_H
