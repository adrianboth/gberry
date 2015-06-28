#include <gtest/gtest.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include <testutils/waiter.h>
#include <testutils/qtgtest.h>

#include "utils/util_messageparser.h"

#include "common/messagefactory.h"

using namespace GBerry::Console;

TEST(MessageFactory, PingMessage)
{
    QByteArray pingMsg = MessageFactory::createPingCommand();

    QJsonParseError errors;
    QJsonDocument doc(QJsonDocument::fromJson(pingMsg, &errors));
    ASSERT_TRUE(errors.error == QJsonParseError::NoError);

    QJsonObject json(doc.object());
    ASSERT_TRUE(json.contains("command"));
    ASSERT_TRUE(json["command"] == "Ping");

// -- with code
    pingMsg = MessageFactory::createPingCommand("1234");

    json = QJsonDocument::fromJson(pingMsg).object();
    ASSERT_TRUE(json.contains("application_code"));
    ASSERT_TRUE(json["application_code"] == "1234");
}

TEST(MessageFactory, PingReplyMessage)
{
    QByteArray pingMsg = MessageFactory::createPingReply();

    QJsonParseError errors;
    QJsonDocument doc(QJsonDocument::fromJson(pingMsg, &errors));
    ASSERT_TRUE(errors.error == QJsonParseError::NoError);

    QJsonObject json(doc.object());
    ASSERT_TRUE(json.contains("command"));
    ASSERT_TRUE(json["command"] == "PingReply");

// -- with code
    pingMsg = MessageFactory::createPingReply("1234");

    json = QJsonDocument::fromJson(pingMsg).object();
    ASSERT_TRUE(json.contains("application_code"));
    ASSERT_TRUE(json["application_code"] == "1234");
}

TEST(MessageFactory, PlayerMessage)
{
    QJsonObject playerJson;
    playerJson["test"] = "ok";

    QByteArray playerMsg(MessageFactory::createPlayerMessage(playerJson));
    UtilMessageParser parser(playerMsg);
    ASSERT_TRUE(parser.isOk());
    EXPECT_TRUE(parser.isCommand("PlayerMessage"));
    EXPECT_TRUE(parser.json.contains("data"));
    ASSERT_TRUE(parser.json["data"].isString());
    // content is always a string
    QJsonObject content(QJsonDocument::fromJson(parser.json["data"].toString().toLatin1()).object());

    EXPECT_TRUE(content["test"] == "ok");

// -- string
     parser.parse(MessageFactory::createPlayerMessage("foo"));
     ASSERT_TRUE(parser.isOk());
     EXPECT_TRUE(parser.isCommand("PlayerMessage"));
     EXPECT_TRUE(parser.json.contains("data"));
     ASSERT_TRUE(parser.json["data"].isString());
     EXPECT_TRUE(parser.json["data"].toString() == "foo");
}
