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
#include <server/serversidecontrolchannel.h>
#include <server/playersessionmanager.h>
#include "realsystemservices.h"
#include "localapplicationsstorage.h"
#include "application.h"
#include "server/application/applicationmeta.h"
#include "utils/fileutils.h"

#include <gmock/gmock.h>
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;


#include "mocks/mock_invocationfactory.h"
#include "mocks/mock_downloadstreaminvocation.h"
#include "mocks/mock_restinvocation.h"

#define LOG_AREA "TestDownloadApplicationCommand"
#include "log/log.h"

#include "testutils/jsonutils.h"
#include "testobjects/stub_channelsouthpartner.h"

using namespace GBerry;
using namespace GBerryConsole::Test;

// TODO: other integration tests have this same ... duplicate


// in case of failure temporary app dir is deleted
TEST(DownloadApplicationCommand, DownloadFailsImmediatelyToInvocation)
{
    RealSystemServices systemServices;
    systemServices.registerItself();

    DownloadableApplicationCache applicationCache; // so simple that no mock need ...

    // there needs to be ready cached app
    ApplicationMeta* meta = new ApplicationMeta;
    Application* app(new Application(QSharedPointer<ApplicationMeta>(meta)));
    const QString APPLICATION_ID("TestOffering");
    const QString APPLICATION_VERSION("1.0");
    const QString APPLICATION_FULL_ID(APPLICATION_ID + "-" + APPLICATION_VERSION);
    meta->setApplicationId(APPLICATION_ID);
    meta->setVersion(APPLICATION_VERSION);
    meta->setName("Test Offering");

    applicationCache.cacheApplication(QSharedPointer<IApplication>(static_cast<IApplication*>(app)));

    MockInvocationFactory mockInvocationFactory;
    MockDownloadStreamInvocation* mockInvocation = new MockDownloadStreamInvocation; // this will be deleted
    MockRESTInvocation* mockRESTInvocation = new MockRESTInvocation;

    HeadServerConnection headServerConnection(&mockInvocationFactory);

    StubChannelSouthPartner* stubChannelSouthPartner = new StubChannelSouthPartner; // channel takes ownership

    ServerSideControlChannel controlChannel;
    controlChannel.attachSouthPartner(stubChannelSouthPartner);

    QTemporaryDir tempDir;
    QDir tempDirObj(tempDir.path());
    ASSERT_TRUE(tempDirObj.mkdir("storage"));
    QString storageFilePath = tempDirObj.filePath("storage");
    const QString EXPECTED_APP_DIR_PATH = QDir(storageFilePath).filePath(APPLICATION_FULL_ID);
    DEBUG("Test EXPECTED_APP_DIR_PATH =" << EXPECTED_APP_DIR_PATH);

    LocalApplicationsStorage appsStorage(storageFilePath);
    PlayerSessionManager playerSessions;

    DownloadApplicationCommand* cmd =
            new DownloadApplicationCommand(&headServerConnection, &controlChannel, &applicationCache, &appsStorage, &playerSessions);
    controlChannel.registerCommand(cmd); // channel will take ownership

    // ping setup
    EXPECT_CALL(mockInvocationFactory, newRESTInvocation()).WillOnce(Return(mockRESTInvocation));
    EXPECT_CALL(*mockRESTInvocation, defineGetOperation(QString("/ping/"))).Times(1);
    EXPECT_CALL(*mockRESTInvocation, execute()).Times(1);

    // download setup
    EXPECT_CALL(mockInvocationFactory, newDownloadStreamInvocation()).WillOnce(Return(mockInvocation));
    const QString EXPECTED_URL("/application/download/" + APPLICATION_ID + "/" + APPLICATION_VERSION);
    EXPECT_CALL(*mockInvocation, defineGetOperation(EXPECTED_URL)).Times(1);
    EXPECT_CALL(*mockInvocation, setOutputFilePath(_)).Times(1);

    bool downloadExecuteCalled;
    auto ExecuteCalledFunc = [&] () { downloadExecuteCalled = true; };
    EXPECT_CALL(*mockInvocation, execute()).WillOnce(InvokeWithoutArgs(ExecuteCalledFunc));
    EXPECT_CALL(*mockInvocation, result()).WillOnce(Return(Result(Error(100)))); // just dummy error
    EXPECT_CALL(*mockInvocation, statusCode()).WillOnce(Return(Invocation::CONNECTION_FAILED));

// -- test
    QJsonObject json;
    json["command"] = "DownloadApplication";
    json["application_id"] = APPLICATION_FULL_ID; // there needs to exists app with this (in cache)

    controlChannel.receiveMessageFromSouth(QJsonDocument(json).toJson());

    // first waits ping (TODO: best would be somehow skip pinging ...)
    Mock::VerifyAndClearExpectations(&mockRESTInvocation); // ping invocation ok
    mockRESTInvocation->emitFinishedOK();
    WAIT_AND_ASSERT(headServerConnection.isConnected());

    // connection is now established -> wait download invocation
    WAIT_AND_ASSERT(downloadExecuteCalled);

    Mock::VerifyAndClearExpectations(&mockInvocationFactory);
    Mock::VerifyAndClearExpectations(&mockInvocation);

    ASSERT_TRUE(QDir(EXPECTED_APP_DIR_PATH).exists());

    // After failed downloadstream (because of connection error) HeadServerConnection
    // will initiate a new ping() to validate connection. In our test we are not
    // interested from that functionality but we want to keep mock calls clean
    EXPECT_CALL(mockInvocationFactory, newRESTInvocation()).WillOnce(ReturnNull());

    mockInvocation->emitFinishedError();

    WAIT_CUSTOM_AND_ASSERT(stubChannelSouthPartner->channelSendMessageToSouthCallCount > 0, 15000, 250);

    // we are expecting single message (error)
    ASSERT_TRUE(stubChannelSouthPartner->channelSendMessageToSouthCallCount == 1);

    QJsonObject answerJson = QJsonDocument::fromJson(stubChannelSouthPartner->receivedMessages.first()).object();
    //JsonUtils::debugPrint("DownloadApplicationReply", answerJson);

    // all reply should be this type
    EXPECT_TRUE(answerJson["command"].toString() == "DownloadApplicationReply");
    EXPECT_TRUE(answerJson["application_id"].toString() == APPLICATION_FULL_ID);
    ASSERT_TRUE(answerJson["result"].toString() == "failure");
    EXPECT_TRUE(answerJson.contains("result_details"));

    // and directory is deleted
    ASSERT_FALSE(QDir(EXPECTED_APP_DIR_PATH).exists());
}
