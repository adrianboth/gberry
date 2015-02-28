#include <gtest/gtest.h>

#include <QObject>

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "server/serversidechannelmanager.h"

#include "utils/testchannel.h"


TEST(ServerSideChannelManager, registerHandlerAndWriteAndReadMessages)
{
    ServerSideChannelManager manager;
    int msgChannelId = manager.nextFreeChannelId();
    EXPECT_EQ(msgChannelId, 1); // 0 is reserved for root control channel

    int msgsWritten = 0;
    int channelId = -1;
    QString writtenMsg;
    auto writeFunc = [&] (int cid, const QByteArray msg) {
        msgsWritten++;
        channelId = cid;
        writtenMsg = QString(msg);
    };
    QObject::connect(&manager, &ChannelManager::outgoingMessage, writeFunc);

    TestChannel handler(msgChannelId);
    manager.registerChannel(&handler);

    QByteArray msg("ping");
    manager.processMessage(msgChannelId, msg);

    Waiter::wait([&] () { return handler.received > 0; });
    EXPECT_EQ(handler.received, 1);
    EXPECT_TRUE(handler.lastReceivedMsg == "ping");

    QByteArray msg2("foobar");
    handler.triggerOutgoingMessage(msg2);

    Waiter::wait([&] () { return msgsWritten > 0; });
    ASSERT_EQ(msgsWritten, 1);
    EXPECT_EQ(msgChannelId, channelId);
    EXPECT_TRUE(writtenMsg == "foobar");

    Channel* unregisteredHandler = manager.unregisterChannel(handler.channelId());
    EXPECT_TRUE(unregisteredHandler == &handler); // comparing pointers

    // send a message again (just repeating)
    handler.triggerOutgoingMessage(msg2);
    Waiter::wait([&] () { return msgsWritten > 1; });
    ASSERT_EQ(msgsWritten, 2);
}

// TODO: open channel handshaking
//       - state
