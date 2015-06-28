#include <testutils/qtgtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include <testutils/waiter.h>

#include "server/serverchannelmanager.h"
#include "server/serversidecontrolchannel.h"
#include "server/channelfactory.h"
#include "common/messagefactory.h"

#include "utils/testchannel.h"
#include "utils/testchannelfactory.h"
#include "utils/testplayerchannelnorthpartner.h"
#include "utils/util_messageparser.h"

#define LOG_AREA "test_serverchannelmanager"
#include "log/log.h"

using namespace GBerry::Console;
using namespace GBerry::Console::Server;

TEST(ServerChannelManager, RegisterHandlerAndWriteAndReadMessages)
{
    TRACE("TEST START");

    TestChannelFactory factory;
    ServerChannelManager manager(&factory);
    int msgChannelId = manager.nextFreeChannelId();
    EXPECT_EQ(msgChannelId, 1); // 0 is reserved for root control channel

    int msgsWritten = 0;
    int channelId = -1;
    int connectionId = -1;
    QString writtenMsg;
    auto writeFunc = [&] (int connectionId_, int channelId_, const QByteArray msg) {
        msgsWritten++;
        connectionId = connectionId_;
        channelId = channelId_;
        writtenMsg = QString(msg);
    };
    QObject::connect(&manager, &ServerChannelManager::outgoingMessageToSouth, writeFunc);

    int fakeConnectionId = 0;
    ServerSideControlChannel* controlChannel = manager.openControlChannel(fakeConnectionId);
    ASSERT_TRUE(controlChannel->isOpen());

    // we don't activate channel, we left it for other testcase

    bool channelClosedCalled = false;
    auto closedFunc = [&] () { channelClosedCalled = true; };
    QObject::connect(controlChannel, &ServerSideControlChannel::channelClosed, closedFunc);

// -- send messages, expect pingreply
    QByteArray pingMsg(MessageFactory::createPingCommand());
    manager.processMessageFromSouth(fakeConnectionId, ServerSideControlChannel::CHANNEL_ID, pingMsg);

    Waiter::wait([&] () { return msgsWritten == 1; });
    ASSERT_EQ(msgsWritten, 1);
    DEBUG("Reply msg:" << MessageFactory::createPingReply());
    ASSERT_TRUE(writtenMsg == QString(MessageFactory::createPingReply())) << writtenMsg;
    ASSERT_EQ(connectionId, fakeConnectionId);
    ASSERT_EQ(channelId, ServerSideControlChannel::CHANNEL_ID);

// -- message to other directions
    controlChannel->pingSouth();

    Waiter::wait([&] () { return msgsWritten == 2; });
    ASSERT_EQ(msgsWritten, 2);
    ASSERT_TRUE(writtenMsg == QString(MessageFactory::createPingCommand())) << writtenMsg;
    ASSERT_EQ(connectionId, fakeConnectionId);
    ASSERT_EQ(channelId, ServerSideControlChannel::CHANNEL_ID);

// -- messages sent both directions -> tear down

    manager.applicationClosed(fakeConnectionId);
    Waiter::wait([&] () { return channelClosedCalled; });
    EXPECT_TRUE(channelClosedCalled);
    controlChannel = nullptr;

// -- send a message again -> should not send it forwards (everything unregistered)
    manager.processMessageFromSouth(fakeConnectionId, ServerSideControlChannel::CHANNEL_ID, pingMsg);
    QCoreApplication::processEvents();
    ASSERT_EQ(msgsWritten, 2); // no new messages

    TRACE("TEST DONE")
}


TEST(ServerChannelManager, NewPlayerChannelOpenedAndClosed)
{
    TestChannelFactory factory;

    ServerChannelManager manager(&factory);
    int msgChannelId = manager.nextFreeChannelId();
    EXPECT_EQ(msgChannelId, 1); // 0 is reserved for root control channel

    UtilMessageParser parser;
    int msgsWritten = 0;
    int channelId = -1;
    int connectionId = -1;
    QString writtenMsg;
    auto writeFunc = [&] (int connectionId_, int channelId_, const QByteArray msg) {
        msgsWritten++;
        connectionId = connectionId_;
        channelId = channelId_;
        writtenMsg = QString(msg);
        parser.parse(msg);
    };
    QObject::connect(&manager, &ServerChannelManager::outgoingMessageToSouth, writeFunc);

// -- activate connection

    // because player channel needs active connection -> create such
    int fakeConnectionId = 0;
    manager.openControlChannel(fakeConnectionId);
    manager.activateConnection(fakeConnectionId);

    Waiter::wait([&] () { return msgsWritten == 1; });
    ASSERT_TRUE(msgsWritten == 1);
    ASSERT_TRUE(parser.isOk());
    ASSERT_TRUE(parser.isCommand("Activate"));
    EXPECT_EQ(channelId, 0);

// -- open player channel

    PlayerMeta playerMeta(1, "FooPlayer");
    ServerSidePlayerChannel* playerChannel = manager.openPlayerChannel(playerMeta);

    // note that channel takes ownership
    TestPlayerChannelNorthPartner* northPartner = new TestPlayerChannelNorthPartner(playerChannel->channelId());
    playerChannel->attachNorthPartner(northPartner);

    // playerChannel sends open message, is not open before
    ASSERT_FALSE(playerChannel->isOpen());

    Waiter::wait([&] () { return msgsWritten == 2; });
    ASSERT_TRUE(msgsWritten == 2);
    ASSERT_TRUE(parser.isOk());
    ASSERT_TRUE(parser.isCommand("OpenChannel"));
    ASSERT_TRUE(parser.json.contains("player_metadata")) << writtenMsg;
    EXPECT_TRUE(parser.json["player_metadata"].toObject()["id"] == playerMeta.playerId());
    EXPECT_TRUE(parser.json["player_metadata"].toObject()["name"] == playerMeta.playerName());
    EXPECT_EQ(channelId, playerChannel->channelId());

// -- response to open channel
    QByteArray openChannelReplyMsg(MessageFactory::createOpenChannelReply());
    manager.processMessageFromSouth(fakeConnectionId, playerChannel->channelId(), openChannelReplyMsg);

    Waiter::wait([&] () { return playerChannel->isOpen(); });
    ASSERT_TRUE(playerChannel->isOpen());

// -- message from south
    ASSERT_TRUE(northPartner->messageCount == 0);
    QJsonObject playerJson;
    playerJson["test"] = "ok";

    QByteArray playerMsg(MessageFactory::createPlayerMessage(playerJson));
    manager.processMessageFromSouth(fakeConnectionId, playerChannel->channelId(), playerMsg);

    Waiter::wait([&] () { return northPartner->messageCount == 1; });
    ASSERT_TRUE(northPartner->messageCount == 1);
    UtilMessageParser northParser(northPartner->lastMessage);
    DEBUG("lastMessage:" << northPartner->lastMessage);
    ASSERT_TRUE(northParser.isOk()) << northParser.errorString();
    EXPECT_TRUE(northParser.json.contains("test"));
    EXPECT_TRUE(northParser.json["test"] == "ok");

// -- message from north
    playerChannel->receivePlayerMessageFromNorth(QJsonDocument(playerJson).toJson());
    Waiter::wait([&] () { return msgsWritten == 3; });
    ASSERT_TRUE(msgsWritten == 3);
    ASSERT_TRUE(parser.isOk());
    ASSERT_TRUE(parser.isCommand("PlayerMessage"));
    EXPECT_EQ(channelId, playerChannel->channelId());
    EXPECT_TRUE(parser.json.contains("data"));
    DEBUG("writtenMsg:" << writtenMsg);
    DEBUG("data:" << parser.json["data"].toString());
    QJsonObject pjson = QJsonDocument::fromJson(parser.json["data"].toString().toLatin1()).object();

    EXPECT_TRUE(pjson.contains("test"));
    EXPECT_TRUE(pjson["test"] == "ok");

// -- close channel from north
    manager.closePlayerChannel(playerChannel->channelId());
    Waiter::wait([&] () { return msgsWritten == 4; });
    ASSERT_TRUE(msgsWritten == 4);
    ASSERT_TRUE(parser.isOk());
    ASSERT_TRUE(parser.isCommand("CloseChannel"));

}

// TODO: open channel handshaking

