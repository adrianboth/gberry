#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::_;

#include <QLatin1Char>
#include <QCoreApplication>
#include <QTime>
#include <QThread>
#include <QTemporaryFile>

#include "invocationfactoryimpl.h"
#include "downloadstreaminvocation.h"
#include "testutils/signalrecorder.h"
#include "utils/testhttpserver.h"

#define LOG_AREA "DownloadStreamInvocationTests"
#include "log/log.h"


TEST(DownloadStreamInvocation, OkOperationAllDataByOnce)
{
    InvocationFactoryImpl factoryObj;
    InvocationFactory* factory = &factoryObj; // operate through public interface

    factory->setProperty("url_prefix", "http://localhost:9999/gberryrest/v1");

    DownloadStreamInvocation* inv = factory->newDownloadStreamInvocation();

    int finishedOkCalled = 0;
    int finishedErrorCalled = 0;

    QObject::connect(inv, &DownloadStreamInvocation::finishedOK,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedOkCalled++; });

    QObject::connect(inv, &DownloadStreamInvocation::finishedError,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedErrorCalled++; });

    // set http server to localhost
    TestHttpServer server(9999);

    bool requestReceived = false;
    QString expectedPath("/gberryrest/v1/download");
    auto mySlot = [&](QHttpRequest* req, QHttpResponse* resp)
    {
        requestReceived = true;
        if (req->path() != expectedPath)
        {
            resp->writeHead(403);
            QString errorMsg("Expected path: " + expectedPath + "\nGot path: " + req->path());
            resp->end(errorMsg.toLatin1());
            return;
        }

        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);

        QString body = "data123";
        // TODO: response only partial and somehow later return rest;
        resp->end(body.toUtf8());
    };

    QObject::connect(&server, &TestHttpServer::request, mySlot);

    // tmp file name is generated when first time opened -> open to get name
    QTemporaryFile tmpFile;
    tmpFile.open();
    QString tmpFileName(tmpFile.fileName());
    tmpFile.close();

// -- test
    inv->setOutputFilePath(tmpFileName);
    inv->defineGetOperation("/download");
    inv->execute();

    ASSERT_EQ(Invocation::ONGOING, inv->statusCode());
    WAIT_AND_ASSERT(requestReceived);
    WAIT_AND_ASSERT(finishedOkCalled == 1);
    ASSERT_EQ(0, finishedErrorCalled);

    //Waiter::wait([&] () { return replyReceived;}, true, 15000);
    //Waiter::wait([&] () { return finishedOkCalled == 1;}, true, 15000);

    ASSERT_TRUE(tmpFile.open());
    QByteArray writtenData = tmpFile.readAll();
    tmpFile.close();

    ASSERT_TRUE(writtenData == QByteArray("data123")) << QString(writtenData);
    EXPECT_EQ(Invocation::FINISHED, inv->statusCode());
    EXPECT_EQ(HTTPInvocationDefinition::OK_200, inv->responseHttpStatusCode());
}


TEST(DownloadStreamInvocation, OkOperationDataReadInParts)
{
    InvocationFactoryImpl factoryObj;
    InvocationFactory* factory = &factoryObj; // operate through public interface

    factory->setProperty("url_prefix", "http://localhost:9999/gberryrest/v1");

    DownloadStreamInvocation* inv = factory->newDownloadStreamInvocation();

    int finishedOkCalled = 0;
    int finishedErrorCalled = 0;
    int downloadStartedCalled = 0;
    int downloadProgressCalled = 0;

    QObject::connect(inv, &DownloadStreamInvocation::finishedOK,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedOkCalled++; });

    QObject::connect(inv, &DownloadStreamInvocation::finishedError,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedErrorCalled++; });

    QObject::connect(inv, &DownloadStreamInvocation::downloadStarted,
                     [&] (DownloadStreamInvocation* inv) { Q_UNUSED(inv); downloadStartedCalled++; });

    QObject::connect(inv, &DownloadStreamInvocation::downloadProgress,
                     [&] (DownloadStreamInvocation* inv) { Q_UNUSED(inv); downloadProgressCalled++; });

    // set http server to localhost
    TestHttpServer server(9999);

    bool requestReceived = false;
    QString expectedPath("/gberryrest/v1/download");
    QHttpResponse* httpResponse = nullptr;

    QList<QLatin1String> expectedData;
    expectedData << QLatin1String("data123");
    expectedData << QLatin1String("abcd567");
    expectedData << QLatin1String("foobar9");
    int fullLength = expectedData.at(0).size() + expectedData.at(1).size() + expectedData.at(2).size();

    auto myRequestSlot = [&](QHttpRequest* req, QHttpResponse* resp) {
        requestReceived = true;
        httpResponse = resp;
        if (req->path() != expectedPath)
        {
            resp->writeHead(403);
            QString errorMsg("Expected path: " + expectedPath + "\nGot path: " + req->path());
            resp->end(errorMsg.toLatin1());
            return;
        }

        resp->setHeader("Content-Type", "application/octet-stream");
        resp->setHeader("Content-Length", QString::number(fullLength));
        resp->writeHead(200);
        resp->write(expectedData.at(0).data());
    };

    int allBytesWrittenCalled = 0;
    auto myAllBytesWrittenSlot = [&] () {
        allBytesWrittenCalled++;
    };

    QObject::connect(&server, &TestHttpServer::request, myRequestSlot);
    QObject::connect(&server, &TestHttpServer::request, myAllBytesWrittenSlot);

    // tmp file name is generated when first time opened -> open to get name
    QTemporaryFile tmpFile;
    tmpFile.open();
    QString tmpFileName(tmpFile.fileName());
    tmpFile.close();

// -- test
    inv->setOutputFilePath(tmpFileName);
    inv->defineGetOperation("/download");
    inv->execute();

    ASSERT_EQ(Invocation::ONGOING, inv->statusCode());
    WAIT_AND_ASSERT(requestReceived);

    // should have written first part of response
    WAIT_AND_ASSERT(allBytesWrittenCalled == 1); // let http server write
    WAIT_AND_ASSERT(downloadStartedCalled == 1); // client got something
    WAIT_AND_ASSERT(downloadProgressCalled > 0);
    int oldProgressCount = downloadProgressCalled;

    EXPECT_EQ(33, inv->progressPercentage()) << inv->progressPercentage();

    httpResponse->write(expectedData.at(1).data());

    // TODO: something wrong with webserver allBytesWritten, not signaled second time?
    //WAIT_WITH_TIMEOUT(allBytesWrittenCalled == 1, 5000);  // let http server write

    DEBUG("allBytesWrittenCalled" << allBytesWrittenCalled);
    WAIT_AND_ASSERT(downloadProgressCalled > oldProgressCount); // client got something
    oldProgressCount = downloadProgressCalled;

    EXPECT_EQ(66, inv->progressPercentage()) << inv->progressPercentage();
    EXPECT_EQ(1, downloadStartedCalled); // no new calls

    httpResponse->write(expectedData.at(2).data());
    httpResponse->end();

    //WAIT_AND_ASSERT(allBytesWrittenCalled == 3);
    WAIT_AND_ASSERT(downloadProgressCalled > oldProgressCount);
    WAIT_AND_ASSERT(finishedOkCalled == 1);
    ASSERT_EQ(0, finishedErrorCalled);
    EXPECT_EQ(100, inv->progressPercentage()) << inv->progressPercentage();

    // check saved data

    ASSERT_TRUE(tmpFile.open());
    QByteArray writtenData = tmpFile.readAll();
    tmpFile.close();

    ASSERT_TRUE(writtenData == QByteArray("data123abcd567foobar9")) << QString(writtenData);
    EXPECT_EQ(Invocation::FINISHED, inv->statusCode());
    EXPECT_EQ(HTTPInvocationDefinition::OK_200, inv->responseHttpStatusCode());
}


TEST(DownloadStreamInvocation, FailedDownload)
{
    InvocationFactoryImpl factoryObj;
    InvocationFactory* factory = &factoryObj; // operate through public interface

    factory->setProperty("url_prefix", "http://localhost:9999/gberryrest/v1");

    DownloadStreamInvocation* inv = factory->newDownloadStreamInvocation();

    int finishedOkCalled = 0;
    int finishedErrorCalled = 0;

    QObject::connect(inv, &DownloadStreamInvocation::finishedOK,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedOkCalled++; });

    QObject::connect(inv, &DownloadStreamInvocation::finishedError,
                     [&] (Invocation* inv) { Q_UNUSED(inv); finishedErrorCalled++; });

    // set http server to localhost
    TestHttpServer server(9999);

    bool requestReceived = false;
    auto mySlot = [&](QHttpRequest* req, QHttpResponse* resp)
    {
        Q_UNUSED(req);
        requestReceived = true;
        resp->writeHead(403);
        QString errorMsg("test failure");
        resp->end(errorMsg.toLatin1());
    };

    QObject::connect(&server, &TestHttpServer::request, mySlot);

// -- test
    inv->setOutputFilePath("/foo"); // should not write anything
    inv->defineGetOperation("/download");
    inv->execute();

    ASSERT_EQ(Invocation::ONGOING, inv->statusCode());

    WAIT_AND_ASSERT(requestReceived);
    WAIT_AND_ASSERT(finishedErrorCalled == 1);
    ASSERT_EQ(0, finishedOkCalled);

    EXPECT_EQ(Invocation::ERROR, inv->statusCode());
    EXPECT_EQ(HTTPInvocationDefinition::FORBIDDEN_403, inv->responseHttpStatusCode());
}



// TODO: test case data in parts
//  -- from internet how to provide headers
