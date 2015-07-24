/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include <testutils/qtgtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include <testutils/waiter.h>

#include "client/clientchannelmanager.h"
#include "client/clientsideplayerchannel.h"
#include "client/clientsidecontrolchannel.h"
#include "common/playermeta.h"
#include "common/messagefactory.h"

#include "utils/testclientsidechannelpartners.h"
#include "utils/util_messageparser.h"

#define LOG_AREA "test_clientchannelmanager"
#include "log/log.h"

using namespace GBerry::Console;

TEST(ClientChannelManager, OpeningControlChannel)
{
    ClientChannelManager manager;

    int outgoingMessagesCalled = 0;
    int outgoingMessageChannelId = -1;
    QByteArray lastOutgoingMessage;
    QObject::connect(&manager, &ClientChannelManager::outgoingMessage,
            [&] (int channelId, const QByteArray& msg) {
        outgoingMessagesCalled++;
        outgoingMessageChannelId = channelId;
        lastOutgoingMessage = msg;
    });

    ClientSideControlChannel* controlChannel = new ClientSideControlChannel;
    controlChannel->setApplicationCode("1234");

    int pingReceived = 0;
    QObject::connect(controlChannel, &ClientSideControlChannel::pingReceived,
                     [&] () { pingReceived++; });

    manager.registerControlChannel(controlChannel);

// -- receive ping
    manager.processMessage(ClientSideControlChannel::CHANNEL_ID, MessageFactory::createPingCommand());

    WAIT_AND_ASSERT(pingReceived == 1);
    WAIT_AND_ASSERT(outgoingMessagesCalled == 1);
    EXPECT_EQ(outgoingMessageChannelId, ClientSideControlChannel::CHANNEL_ID);

    QJsonObject json(QJsonDocument::fromJson(lastOutgoingMessage).object());
    EXPECT_TRUE(json["command"] == "PingReply") << QString(lastOutgoingMessage);
    EXPECT_TRUE(json["application_code"] == "1234") << QString(lastOutgoingMessage);

// -- ping
    controlChannel->ping();
    WAIT_AND_ASSERT(outgoingMessagesCalled == 2);
    json = QJsonDocument::fromJson(lastOutgoingMessage).object();
    EXPECT_TRUE(json["command"] == "Ping");


    // give reply
    manager.processMessage(ClientSideControlChannel::CHANNEL_ID, MessageFactory::createPingReply());
    WAIT_AND_ASSERT(pingReceived == 2);
    WAIT_AND_ASSERT(outgoingMessagesCalled == 2); // no replies

}

TEST(ClientChannelManager, NewClientSidePlayerChannelOpenedAndClosed)
{
    ClientChannelManager manager;

    int newChannelIdReceived = -1;
    ClientSidePlayerChannel* playerChannel(nullptr);

    QObject::connect(&manager, &ClientChannelManager::newPlayerChannel,
                     [&] (ClientSidePlayerChannel* channel, PlayerMeta playerMeta)
    {
        Q_UNUSED(playerMeta);
        newChannelIdReceived = channel->channelId();
        playerChannel = channel;
    });

    int outgoingMessagesCalled = 0;
    int outgoingMessageChannelId = -1;
    QByteArray lastOutgoingMessage;
    QObject::connect(&manager, &ClientChannelManager::outgoingMessage,
            [&] (int channelId, const QByteArray& msg) {
        outgoingMessagesCalled++;
        outgoingMessageChannelId = channelId;
        lastOutgoingMessage = msg;
    });

    PlayerMeta pmeta(100, "fooname");
    manager.processMessage(1, MessageFactory::createOpenChannelCommand(pmeta));

    WAIT_AND_ASSERT(newChannelIdReceived == 1);
    ASSERT_TRUE(playerChannel != nullptr);
    ASSERT_TRUE(outgoingMessagesCalled == 1);
    ASSERT_TRUE(outgoingMessageChannelId == playerChannel->channelId());
    QByteArray openChannelReplyMsg = MessageFactory::createOpenChannelReply();
    ASSERT_TRUE(lastOutgoingMessage == openChannelReplyMsg) << QString(openChannelReplyMsg);

// -- player message to application

    // note playerChannel will own this object
    TestClientSidePlayerChannelPartner* partner = new TestClientSidePlayerChannelPartner(playerChannel->channelId());
    playerChannel->attachPlayerChannelPartner(partner);

    QJsonObject playerJson;
    playerJson["test"] = "ok";

    QByteArray playerMsg(MessageFactory::createPlayerMessage(playerJson));
    manager.processMessage(playerChannel->channelId(), playerMsg);

    WAIT_AND_ASSERT(partner->playerMessageCount == 1);
    UtilMessageParser parser(partner->lastMessage);
    ASSERT_TRUE(parser.isOk());
    ASSERT_TRUE(parser.json.contains("test"));
    ASSERT_TRUE(parser.json["test"] == "ok");

// -- player message to player
    ASSERT_EQ(outgoingMessagesCalled, 1);
    playerChannel->sendPlayerMessage(QJsonDocument(playerJson).toJson());

    WAIT_AND_ASSERT(outgoingMessagesCalled == 2);
    EXPECT_EQ(outgoingMessageChannelId, playerChannel->channelId());
    QString expected = MessageFactory::createPlayerMessage(playerJson);
    EXPECT_TRUE(lastOutgoingMessage == playerMsg) << QString(lastOutgoingMessage) << QString(playerMsg);

// -- player exit
    ASSERT_EQ(partner->playerExitCalledCount, 0);
    manager.processMessage(playerChannel->channelId(), MessageFactory::createCloseChannelCommand());

    WAIT_AND_ASSERT(partner->playerExitCalledCount == 1);
}

// TODO: open channel handshaking (connection test)
//       - state
