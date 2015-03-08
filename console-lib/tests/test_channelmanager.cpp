#include <gtest/gtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include <testutils/signalrecorder.h>
#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "server/serversidechannelmanager.h"

#include "client/clientsidechannelmanager.h"
#include "client/clientsideplayerchannel.h"

#include "utils/testchannel.h"

#define LOG_AREA "test_channelmanager"
#include "log/log.h"


TEST(ServerSideChannelManager, RegisterHandlerAndWriteAndReadMessages)
{
    TRACE("TESTSTART");

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

// -- send messages
    QByteArray msg("ping");
    manager.processMessage(msgChannelId, msg);

    Waiter::wait([&] () { return handler.received > 0; });
    EXPECT_EQ(handler.received, 1);
    EXPECT_TRUE(handler.lastReceivedMsg == "ping");

// message to other directions
    QByteArray msg2("foobar");
    handler.triggerOutgoingMessage(msg2);

    Waiter::wait([&] () { return msgsWritten > 0; });
    ASSERT_EQ(msgsWritten, 1);
    EXPECT_EQ(msgChannelId, channelId);
    EXPECT_TRUE(writtenMsg == "foobar");

// messages sent both directions -> tear down

    Channel* unregisteredHandler = manager.unregisterChannel(handler.channelId());
    EXPECT_TRUE(unregisteredHandler == &handler); // comparing pointers

// send a message again -> should not send it forwards (everything unregistered)
    handler.triggerOutgoingMessage(msg2);
    Waiter::wait([&] () { return msgsWritten > 1; });
    ASSERT_EQ(msgsWritten, 1);
    TRACE("TESTDONE")
}

TEST(ClientSideChannelManager, NewPlayerChannelOpenedAndClosed)
{
    ClientSideChannelManager manager;

    int newChannelIdReceived = -1;
    QObject::connect(&manager, &ClientSideChannelManager::newPlayerChannel,
                     [&] (PlayerChannel* channel, PlayerMetadata playerMeta)
    {
        newChannelIdReceived = channel->channelId();
    });

    int closedChannelIdReceived = -1;
    QObject::connect(&manager, &ClientSideChannelManager::channelClosed,
                     [&] (int cid) { closedChannelIdReceived = cid; });

    QJsonObject jsonOpen;
    jsonOpen["command"] = "OpenChannel";
    PlayerMetadata pmeta(100, "fooname");
    jsonOpen["player_metadata"] = pmeta.json();
    manager.processMessage(1, QJsonDocument(jsonOpen).toJson());

    WAIT_AND_ASSERT(newChannelIdReceived == 1);

    QJsonObject jsonClose;
    jsonClose["command"] = "CloseChannel";
    manager.processMessage(1, QJsonDocument(jsonClose).toJson());
    WAIT_AND_ASSERT(closedChannelIdReceived == 1);
}

// TODO: open channel handshaking
//       - state
