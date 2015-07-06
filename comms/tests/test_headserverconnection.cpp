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
using ::testing::InSequence;

#include "restinvocationfactory.h"
#include "mocks/mock_restinvocationfactory.h"
#include "mocks/mock_restinvocation.h"
#include "testobjects/stub_systemservices.h"

#define LOG_AREA "HeadServerConnectionTests"
#include "log/log.h"

/**
 * NOTE: Because cancel() is "self-desctruct" always use always heap to
 *       allocate ('new' operator).
 */
class TestRequest : public Request
{
public:
    enum RequestResult { NOT_READY, READY_OK, READY_FAILED };
    RequestResult result{NOT_READY};

protected:
    virtual Invocation* processPrepare(RESTInvocationFactory *factory) override {
        RESTInvocation* inv = factory->newRESTInvocation();
        // guard against problems with mocking while developing tests
        if (!inv) {
            WARN("newRESTInvocation() returned NULL");
            return NULL;
        }
        inv->defineGetOperation("/testrequest");
        return inv;
    }

    virtual void processOkResponse(Invocation *invocation) override { result = READY_OK; if (invocation) invocation->deleteLater(); }
    virtual void processErrorResponse(Error err, Invocation *invocation) override { Q_UNUSED(err); result = READY_FAILED; if (invocation) invocation->deleteLater(); }

};


TEST(HeadServerConnection, PingOK)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation* invocationMock = new MockRESTInvocation;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation
    HeadServerConnection conn(&factoryMock);
    ASSERT_FALSE(conn.isConnected());

    QSignalSpy spyConnected(&conn, SIGNAL(connected()));
    QSignalSpy spyDisconnected(&conn, SIGNAL(disconnected()));

    // no calls yet
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);

// -- open and expect ping
    EXPECT_CALL(factoryMock, newRESTInvocation())
              .Times(1)
              .WillOnce(Return(invocationMock));

    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    conn.open(); // should start ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);

    invocationMock->emitFinishedOK(); // ping response

    WAIT_AND_ASSERT(conn.isConnected());
    ASSERT_EQ(1, spyConnected.count());
    ASSERT_EQ(0, spyDisconnected.count());

    delete invocationMock;

// -- expect other ping but fail this time
    invocationMock = new MockRESTInvocation;

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    // verify that single shot timer gets time that is in property
    ASSERT_EQ(testservices.singleShotTimerWaitMs(), conn.property(HeadServerConnection::PING_INTERVAL_INT_PROP).toInt());

    // fast forward time to trigger single shot
    testservices.invokeSingleshotTimer();

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock); // is "ping" done

    invocationMock->emitFinishedError(); // ping fails

    WAIT_AND_ASSERT(!conn.isConnected());
    ASSERT_EQ(1, spyConnected.count()); // no new connects
    ASSERT_EQ(1, spyDisconnected.count());

    delete invocationMock;

// -- one more ping, ok -> connected
    invocationMock = new MockRESTInvocation;

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    // fast forward time to trigger single shot
    testservices.invokeSingleshotTimer();

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock); // is "ping" done

    invocationMock->emitFinishedOK(); // ping fails

    WAIT_AND_ASSERT(conn.isConnected());
    ASSERT_EQ(2, spyConnected.count());
    ASSERT_EQ(1, spyDisconnected.count()); // no new disconnects
}


TEST(HeadServerConnection, MakeRequestWhenConnectionOK)
{
    MockRESTInvocationFactory factoryMock;

    // We need to create fresh RESTInvocation for each request because
    // otherwise signal connection would pile up and as HeadServerConnection
    // attaches to signals it would get extra signals
    MockRESTInvocation* invocationMock = new MockRESTInvocation;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation and drive object to connected state
    HeadServerConnection conn(&factoryMock);

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    conn.open(); // should make initial ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock); // is "ping" done

    invocationMock->emitFinishedOK(); // ping response
    WAIT_AND_ASSERT(conn.isConnected());

    delete invocationMock;

// -- ok request
    invocationMock = new MockRESTInvocation;

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/testrequest"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    TestRequest* request = new TestRequest; // always allocate in heap
    ASSERT_EQ(TestRequest::NOT_READY, request->result);

    conn.makeRequest(request);

    invocationMock->emitFinishedOK();

    WAIT_AND_ASSERT(request->result == TestRequest::READY_OK);
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);

    delete invocationMock;
    delete request;

// -- failed request (invocation failure)
    TRACE("Test: Failed request");

    invocationMock = new MockRESTInvocation;
    request = new TestRequest; // always allocate in heap

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/testrequest"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);
    EXPECT_CALL(*invocationMock, statusCode()).Times(1).WillOnce(Return(Invocation::ERROR));

    conn.makeRequest(request);

    // there are different behaviour on different errors -> now generic error
    // is what mock returns -> no extra ping
    invocationMock->emitFinishedError();

    WAIT_AND_ASSERT(request->result == TestRequest::READY_FAILED);
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);

    delete invocationMock;
    delete request;

// -- failed request to connection error -> ping triggered
    TRACE("Test: Failed requests triggers ping");

    invocationMock = new MockRESTInvocation;
    MockRESTInvocation* pingInvocationMock = new MockRESTInvocation;

    request = new TestRequest; // always allocate in heap

    InSequence dummy; // following expectations will be in sequences
    EXPECT_CALL(factoryMock, newRESTInvocation()).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/testrequest")));
    EXPECT_CALL(*invocationMock, execute()).Times(1);
    EXPECT_CALL(*invocationMock, statusCode()).WillOnce(Return(Invocation::CONNECTION_FAILED));
    EXPECT_CALL(factoryMock, newRESTInvocation()).WillOnce(Return(pingInvocationMock));
    EXPECT_CALL(*pingInvocationMock, defineGetOperation(QString("/ping")));
    EXPECT_CALL(*pingInvocationMock, execute());

    conn.makeRequest(request);

    // there are different behaviour on different errors -> now generic error
    // is what mock returns -> no extra ping
    invocationMock->emitFinishedError();

    WAIT_AND_ASSERT(request->result == TestRequest::READY_FAILED);
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);
    Mock::VerifyAndClearExpectations(pingInvocationMock);

    delete invocationMock;
    delete pingInvocationMock;
    delete request;
}


TEST(HeadServerConnection, MakeRequestWhenConnectionNotOK)
{
    MockRESTInvocationFactory factoryMock;
    MockRESTInvocation* invocationMock = new MockRESTInvocation;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation and drive object to connected state (ping has failed)
    HeadServerConnection conn(&factoryMock);

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    conn.open(); // should make initial ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock); // is "ping" done

    invocationMock->emitFinishedError(); // ping response

    WAIT_AND_ASSERT(!conn.isConnected());

    delete invocationMock;

// -- make request (if no connection tries to run ping first)
TRACE("Test: Make request that will fail because of no connection");

    invocationMock = new MockRESTInvocation;

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    TestRequest request;
    conn.makeRequest(&request);

    invocationMock->emitFinishedError(); // ping still fails

    // and as ping failed also request is failed
    WAIT_AND_ASSERT(request.result == TestRequest::READY_FAILED);
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);
}


TEST(HeadServerConnection, CancelRequestWhenConnectionOK)
{
    MockRESTInvocationFactory factoryMock;

    // We need to create fresh RESTInvocation for each request because
    // otherwise signal connection would pile up and as HeadServerConnection
    // attaches to signals it would get extra signals
    MockRESTInvocation* invocationMock = new MockRESTInvocation;

    TestSystemServices testservices; // for single shot timer faking
    testservices.registerInstance();

// -- creation and drive object to connected state
    HeadServerConnection conn(&factoryMock);

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/ping"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);

    conn.open(); // should make initial ping

    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock); // is "ping" done

    invocationMock->emitFinishedOK(); // ping response
    WAIT_AND_ASSERT(conn.isConnected());

    delete invocationMock;

// -- ok request
    invocationMock = new MockRESTInvocation;

    EXPECT_CALL(factoryMock, newRESTInvocation()).Times(1).WillOnce(Return(invocationMock));
    EXPECT_CALL(*invocationMock, defineGetOperation(QString("/testrequest"))).Times(1);
    EXPECT_CALL(*invocationMock, execute()).Times(1);
    EXPECT_CALL(*invocationMock, abort()).Times(1);

    // in cancel we let Request delete itself
    TestRequest* request = new TestRequest; // always allocate in heap
    conn.makeRequest(request);
    request->cancel();
    QCoreApplication::processEvents();

    invocationMock->emitFinishedOK();

    // when cancelled then actual action is not executed
    WAIT_AND_ASSERT(request->result == TestRequest::NOT_READY);
    Mock::VerifyAndClearExpectations(&factoryMock);
    Mock::VerifyAndClearExpectations(invocationMock);

    delete invocationMock;

}


// TESTCASE: canceling a ping (killing object doesn't crash the system ...)
// TESTCASE: canceling a request

// TESTCASE: make a request
//   -- case when connection ok
//   -- case when ping fails -> request fails

// TESTCASE: any request is valid ping
