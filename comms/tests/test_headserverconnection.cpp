#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <QSignalSpy>

#include "headserverconnection.h"
#include "request.h"
using namespace GBerry;

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "mocks/mock_restinvocationfactory.h"
#include "mocks/mock_restinvocation.h"
#include "testobjects/stub_systemservices.h"


class TestRequest : public Request
{
public:
    enum RequestResult { NOT_READY, READY_OK, READY_FAILED };
    RequestResult result{NOT_READY};

    virtual void prepare(RESTInvocation *invocation) override {
        invocation->get("/testrequest");
    }
    virtual void finishedOk(RESTInvocation *invocation) override { Q_UNUSED(invocation); result = READY_OK; }
    virtual void finishedFailure(RESTInvocation *invocation) override { Q_UNUSED(invocation); result = READY_FAILED; }

    // --
    void reset() { result = NOT_READY; }
};


TEST(HeadServerConnection, PingOK)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation invocationMock;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation
    HeadServerConnection conn(&factoryMock);
    ASSERT_FALSE(conn.isConnected());

    QSignalSpy spyConnected(&conn, SIGNAL(connected()));
    QSignalSpy spyDisconnected(&conn, SIGNAL(disconnected()));

    // no calls yet
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock);

// -- open and expect ping
    EXPECT_CALL(factoryMock, newInvocation())
              .Times(1)
              .WillOnce(Return(&invocationMock));

    EXPECT_CALL(invocationMock, get(QString("/ping")))
              .Times(1);

    conn.open(); // should start ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock);

    invocationMock.emitFinishedOK(); // ping response

    WAIT_AND_ASSERT(conn.isConnected());
    ASSERT_EQ(1, spyConnected.count());
    ASSERT_EQ(0, spyDisconnected.count());

// -- expect other ping but fail this time

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/ping"))).Times(1);

    // verify that single shot timer gets time that is in property
    ASSERT_EQ(testservices.singleShotTimerWaitMs(), conn.property(HeadServerConnection::PING_INTERVAL_INT_PROP).toInt());

    // fast forward time to trigger single shot
    testservices.invokeSingleshotTimer();

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock); // is "ping" done

    invocationMock.emitFinishedError(); // ping fails

    WAIT_AND_ASSERT(!conn.isConnected());
    ASSERT_EQ(1, spyConnected.count()); // no new connects
    ASSERT_EQ(1, spyDisconnected.count());

// -- one more ping, ok -> connected

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/ping"))).Times(1);

    // fast forward time to trigger single shot
    testservices.invokeSingleshotTimer();

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock); // is "ping" done

    invocationMock.emitFinishedOK(); // ping fails

    WAIT_AND_ASSERT(conn.isConnected());
    ASSERT_EQ(2, spyConnected.count());
    ASSERT_EQ(1, spyDisconnected.count()); // no new disconnects
}


TEST(HeadServerConnection, MakeRequestWhenConnectionOK)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation invocationMock;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation and drive object to connected state
    HeadServerConnection conn(&factoryMock);

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/ping"))).Times(1);

    conn.open(); // should make initial ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock); // is "ping" done

    invocationMock.emitFinishedOK(); // ping response

    WAIT_AND_ASSERT(conn.isConnected());

// -- ok request

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/testrequest"))).Times(1);

    TestRequest request;
    ASSERT_EQ(TestRequest::NOT_READY, request.result);

    conn.makeRequest(&request);

    invocationMock.emitFinishedOK();

    WAIT_AND_ASSERT(request.result == TestRequest::READY_OK);
    Mock::VerifyAndClearExpectations(&factoryMock);

// -- failed request
    request.reset();
    ASSERT_EQ(TestRequest::NOT_READY, request.result);

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/testrequest"))).Times(1);

    conn.makeRequest(&request);

    invocationMock.emitFinishedError();

    WAIT_AND_ASSERT(request.result == TestRequest::READY_FAILED);
    Mock::VerifyAndClearExpectations(&factoryMock);
}


TEST(HeadServerConnection, MakeRequestWhenConnectionNotOK)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation invocationMock;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation and drive object to connected state (ping has failed)
    HeadServerConnection conn(&factoryMock);

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/ping"))).Times(1);

    conn.open(); // should make initial ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(&invocationMock); // is "ping" done

    invocationMock.emitFinishedError(); // ping response

    WAIT_AND_ASSERT(!conn.isConnected());

// -- make request (if no connection tries to run ping first)

    EXPECT_CALL(factoryMock, newInvocation()).Times(1).WillOnce(Return(&invocationMock));
    EXPECT_CALL(invocationMock, get(QString("/ping"))).Times(1);

    TestRequest request;
    conn.makeRequest(&request);

    invocationMock.emitFinishedError(); // ping still fails

    // and as ping failed also request is failed
    WAIT_AND_ASSERT(request.result == TestRequest::READY_FAILED);
    Mock::VerifyAndClearExpectations(&factoryMock);
}

// TESTCASE: canceling a ping (killing object doesn't crash the system ...)
// TESTCASE: canceling a request

// TESTCASE: make a request
//   -- case when connection ok
//   -- case when ping fails -> request fails

// TESTCASE: any request is valid ping
