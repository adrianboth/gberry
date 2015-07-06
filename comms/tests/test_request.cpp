
#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;

#include "request.h"
#include "restinvocation.h"
#include "mocks/mock_restinvocation.h"
#include "mocks/mock_restinvocationfactory.h"

// SimpleRequest comes from here
#include "test_request.h"

// basic usage of Request (subclassing)
TEST(Request, OKRequest)
{
    SimpleRequest* req = new SimpleRequest;

    MockRESTInvocation mockInv;
    MockRESTInvocationFactory mockFactory;
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));

// -- test
    Invocation* inv = req->prepareInvocation(&mockFactory);
    req->finishedOk(inv);

    ASSERT_EQ(1, req->ProcessOkCallCount);
    ASSERT_EQ(&mockInv, inv);
    ASSERT_EQ(&mockInv, req->LastInvocation);

    delete req;
}


TEST(Request, FailedRequest)
{
    SimpleRequest* req = new SimpleRequest;

    MockRESTInvocation mockInv;
    MockRESTInvocationFactory mockFactory;
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));

// -- test
    Invocation* inv = req->prepareInvocation(&mockFactory);
    req->finishedError(Request::ERROR_INVOCATION_FAILED, &mockInv);

    ASSERT_EQ(1, req->ProcessErrorCallCount);
    ASSERT_EQ(&mockInv, inv);
    ASSERT_EQ(&mockInv, req->LastInvocation);
    ASSERT_EQ(Request::ERROR_INVOCATION_FAILED, req->LastError);

    delete req;
}

TEST(Request, CancelRequestAndOKResponse)
{
    SimpleRequest* req = new SimpleRequest;

    bool destroyedCalled = false;
    QObject::connect(req, &SimpleRequest::destroyed, [&] () { destroyedCalled = true; });

    MockRESTInvocationFactory mockFactory;
    MockRESTInvocation mockInv;

// --
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));
    EXPECT_CALL(mockInv, abort()).Times(1);
    req->prepareInvocation(&mockFactory);
    req->cancel();
    req->finishedOk(&mockInv);
    ASSERT_EQ(0, req->ProcessOkCallCount);
    ASSERT_EQ(nullptr, req->LastInvocation);

    // req should be now self destroyed
    WAIT_AND_ASSERT(destroyedCalled);
}


TEST(Request, CancelRequestAndErrorResponse)
{
    SimpleRequest* req = new SimpleRequest;

    bool destroyedCalled = false;
    QObject::connect(req, &SimpleRequest::destroyed, [&] () { destroyedCalled = true; });

    MockRESTInvocation mockInv;
    MockRESTInvocationFactory mockFactory;

// --
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));
    EXPECT_CALL(mockInv, abort()).Times(1);
    req->prepareInvocation(&mockFactory);
    req->cancel();

    req->finishedError(Request::ERROR_NO_CONNECTION, &mockInv);
    ASSERT_EQ(0, req->ProcessOkCallCount);
    ASSERT_EQ(0, req->ProcessErrorCallCount);
    ASSERT_EQ(nullptr, req->LastInvocation);

    // req should be now self destroyed
    WAIT_AND_ASSERT(destroyedCalled);
}
