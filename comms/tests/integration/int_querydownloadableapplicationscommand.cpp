#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "commands/querydownloadableapplicationscommand.h"
#include "headserverconnection.h"
#include "invocationfactoryimpl.h"
#include "downloadableapplicationcache.h"
#include "server/serversidecontrolchannel.h"
#include "realsystemservices.h"

#define LOG_AREA "IntegrationQueryDownloadableApplicationsCommand"
#include "log/log.h"

using namespace GBerry;


class TestChannelSouthPartner : public ChannelSouthPartner
{
public:
    TestChannelSouthPartner() {}
    virtual ~TestChannelSouthPartner() {}

    // channel has received closing message from other end
    virtual void channelCloseReceived() override { Q_ASSERT(false); } // should not happen in this tes

    int channelSendMessageToSouthCallCount{0};
    QByteArray lastSentMessage;
    virtual void channelSendMessageToSouth(const QByteArray& msg) override {
        channelSendMessageToSouthCallCount++;
        lastSentMessage = msg;
    }

};

// this is integration test with real local server.
TEST(IntegrationQueryDownloadableApplicationsCommand, OK)
{
    RealSystemServices systemServices;
    systemServices.registerItself();

    DownloadableApplicationCache applicationCache; // so simple that no mock need ...

    InvocationFactoryImpl invocationFactory;
    invocationFactory.setProperty("url_prefix", "http://localhost/gberryrest/v1");

    HeadServerConnection headServerConnection(&invocationFactory); // TODO: some interface for this

    TestChannelSouthPartner* testChannelSouthPartner = new TestChannelSouthPartner; // channel takes ownership

    ServerSideControlChannel controlChannel; // TODO: some interface for message sending
    controlChannel.attachSouthPartner(testChannelSouthPartner);

    QueryDownloadableApplicationsCommand* cmd =
            new QueryDownloadableApplicationsCommand(&headServerConnection, &controlChannel, &applicationCache);
    controlChannel.registerCommand(cmd); // channel will take ownership

// -- test
    QJsonObject json;
    json["command"]  = "QueryDownloadableApplications";

    controlChannel.receiveMessageFromSouth(QJsonDocument(json).toJson());

    WAIT_WITH_TIMEOUT(testChannelSouthPartner->channelSendMessageToSouthCallCount > 0, 30000); // 30s
    ASSERT_EQ(1, testChannelSouthPartner->channelSendMessageToSouthCallCount);

    ASSERT_TRUE(testChannelSouthPartner->lastSentMessage.size() > 0);

    QJsonObject answerJson = QJsonDocument::fromJson(testChannelSouthPartner->lastSentMessage).object();
    EXPECT_TRUE(answerJson["command"].toString() == "QueryDownloadableApplicationsReply");
    EXPECT_TRUE(answerJson["result"].toString() == "ok");
    EXPECT_TRUE(answerJson.contains("applications"));

    // not really interested what apps are there if there are
    // (message syntax is tested elsewhere)
    EXPECT_TRUE(answerJson["applications"].toArray().size() > 0);

    foreach(QJsonValue value, answerJson["applications"].toArray()) {
        QJsonObject appJson = value.toObject();
        QString appFullId = appJson["id"].toString();
        EXPECT_TRUE(appFullId.size() > 0);
        DEBUG("ApplicationFullId:" << appFullId);
    }
}
