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

#define LOG_AREA "IntegrationDownloadApplicationCommand"
#include "log/log.h"

#include "testutils/jsonutils.h"

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
    QList<QByteArray> receivedMessages;

    QByteArray lastSentMessage;
    virtual void channelSendMessageToSouth(const QByteArray& msg) override {
        channelSendMessageToSouthCallCount++;
        receivedMessages.append(msg);
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

    int indexOfProcessMessages = 0;

    // we will receive several messages
    bool waitForMessages = true;
    while (waitForMessages) {
        WAIT_CUSTOM_AND_ASSERT(testChannelSouthPartner->channelSendMessageToSouthCallCount > indexOfProcessMessages, 15000, 250);

        // validate message
        while (indexOfProcessMessages < testChannelSouthPartner->channelSendMessageToSouthCallCount) {
            DEBUG("Validating a message: index =" << indexOfProcessMessages);
            QJsonObject answerJson = QJsonDocument::fromJson(testChannelSouthPartner->receivedMessages.at(indexOfProcessMessages)).object();
            JsonUtils::debugPrint("DownloadApplicationReply", answerJson);

            // all reply should be this type
            EXPECT_TRUE(answerJson["command"].toString() == "DownloadApplicationReply");
            EXPECT_TRUE(answerJson["application_id"].toString() == app->id());

            // first we expect answer that download has started
            // then progress
            // and finally ok
            if (indexOfProcessMessages == 0) {
                ASSERT_TRUE(answerJson["result"].toString() == "status");
                ASSERT_TRUE(answerJson["status"].toString() == "started");
            } else {

                QString resultString(answerJson["result"].toString());
                if (resultString == "status") {
                    ASSERT_TRUE(answerJson["status"].toString() == "progress");
                    ASSERT_TRUE(answerJson.contains("progress_percentage"));

                } else if (resultString == "ok") {
                    // download ok -> no more messages expected
                    waitForMessages = false;
                    break;
                } else {
                    ERROR("Got unknown result:" << resultString);
                    FAIL();
                }

            }

            indexOfProcessMessages++;
        }
    }

    // all messages received, download finished

    QSharedPointer<IApplication> updatedApp = appsStorage.localApplications()->application(app->id());
    EXPECT_TRUE(updatedApp->state() == IApplication::Valid);

    QDir appDir(updatedApp->meta()->applicationDirPath());
    QFileInfo downloadedZip(appDir.filePath("app.zip")); // this is jus known thing
    ASSERT_TRUE(downloadedZip.exists());

    QString extractedFile = GBerryLib::joinpath(appDir.path(), "text.txt"); // this just know piece of test data
    ASSERT_TRUE(QFileInfo(extractedFile).exists());

    //WAIT_WITH_TIMEOUT(false == true, 300000000);

}
