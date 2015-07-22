#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QTemporaryDir>

#include "commands/downloadapplicationcommand.h"
#include "headserverconnection.h"
#include "invocationfactoryimpl.h"
#include "downloadableapplicationcache.h"
#include "server/serversidecontrolchannel.h"
#include "realsystemservices.h"
#include "localapplicationsstorage.h"
#include "application.h"
#include "server/application/applicationmeta.h"
#include "utils/fileutils.h"

#define LOG_AREA "IntegrationyDownloadApplicationCommand"
#include "log/log.h"

using namespace GBerry;

// TODO: other integration tests have this same ... duplicate
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
TEST(DownloadApplicationCommand, DownloadOk)
{
    RealSystemServices systemServices;
    systemServices.registerItself();

    DownloadableApplicationCache applicationCache; // so simple that no mock need ...

    // there needs to be (at this point of implemenation work) ready cached app
    ApplicationMeta* meta = new ApplicationMeta;
    Application* app(new Application(QSharedPointer<ApplicationMeta>(meta)));
    meta->setApplicationId("TestOffering");
    meta->setVersion("1.0");
    meta->setName("Test Offering");

    applicationCache.cacheApplication(QSharedPointer<IApplication>(static_cast<IApplication*>(app)));

    InvocationFactoryImpl invocationFactory;
    invocationFactory.setProperty("url_prefix", "http://localhost/gberryrest/v1");

    HeadServerConnection headServerConnection(&invocationFactory); // TODO: some interface for this

    TestChannelSouthPartner* testChannelSouthPartner = new TestChannelSouthPartner; // channel takes ownership

    ServerSideControlChannel controlChannel; // TODO: some interface for message sending
    controlChannel.attachSouthPartner(testChannelSouthPartner);

    QTemporaryDir tempDir;
    QDir tempDirObj(tempDir.path());
    ASSERT_TRUE(tempDirObj.mkdir("storage"));
    QString storageFilePath = tempDirObj.filePath("storage");

    LocalApplicationsStorage appsStorage(storageFilePath);

    DownloadApplicationCommand* cmd =
            new DownloadApplicationCommand(&headServerConnection, &controlChannel, &applicationCache, &appsStorage);
    controlChannel.registerCommand(cmd); // channel will take ownership

// -- test
    QJsonObject json;
    json["command"] = "DownloadApplication";
    json["application_id"] = app->id(); // there needs to exists app with this

    controlChannel.receiveMessageFromSouth(QJsonDocument(json).toJson());

    WAIT_WITH_TIMEOUT(testChannelSouthPartner->channelSendMessageToSouthCallCount > 0, 30000); // 30s
    ASSERT_EQ(1, testChannelSouthPartner->channelSendMessageToSouthCallCount);

    ASSERT_TRUE(testChannelSouthPartner->lastSentMessage.size() > 0);

    QJsonObject answerJson = QJsonDocument::fromJson(testChannelSouthPartner->lastSentMessage).object();
    EXPECT_TRUE(answerJson["command"].toString() == "DownloadApplicationReply");
    EXPECT_TRUE(answerJson["result"].toString() == "ok") << answerJson["error_string"].toString();
    EXPECT_TRUE(answerJson["application_id"].toString() == json["application_id"].toString());

    QSharedPointer<IApplication> updatedApp = appsStorage.localApplications()->application(app->id());
    EXPECT_TRUE(updatedApp->state() == IApplication::Valid);

    QDir appDir(updatedApp->meta()->applicationDirPath());
    QFileInfo downloadedZip(appDir.filePath("app.zip")); // this is jus known thing
    ASSERT_TRUE(downloadedZip.exists());

    QString extractedFile = GBerryLib::joinpath(appDir.path(), "text.txt"); // this just know piece of test data
    ASSERT_TRUE(QFileInfo(extractedFile).exists());

    //WAIT_WITH_TIMEOUT(false == true, 300000000);

}
