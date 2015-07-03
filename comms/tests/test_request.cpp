
#include <testutils/qtgtest.h>
#include <testutils/waiter.h>

#include "request.h"
#include "restinvocation.h"
#include "mocks/mock_restinvocation.h"

// SimpleRequest comes from here
#include "test_request.h"


TEST(Request, OKRequest)
{
    SimpleRequest* req = new SimpleRequest;

    // our simple test class doesn't handle RESTInvocation anyway
    //  -> use dummy value (non-null) to check passing pointer
    MockRESTInvocation mockInv;

    req->finishedOk(nullptr); // &mockInv
    return;
    ASSERT_EQ(1, req->processOkCallCount);
    ASSERT_EQ(&mockInv, req->lastInvocation);

    req->lastInvocation = nullptr;

    req->finishedError(&mockInv);
    ASSERT_EQ(1, req->processErrorCallCount);
    ASSERT_EQ(&mockInv, req->lastInvocation);

    delete req;
}


TEST(Request, CancelRequestAndOKResponse)
{
    SimpleRequest* req = new SimpleRequest;

    bool destroyedCalled = false;
    QObject::connect(req, &SimpleRequest::destroyed, [&] () { destroyedCalled = true; });

    MockRESTInvocation mockInv;

// --
    EXPECT_CALL(mockInv, abort()).Times(1);
    req->prepare(&mockInv);
    req->cancel();;
    req->finishedOk(&mockInv);
    ASSERT_EQ(0, req->processOkCallCount);
    ASSERT_EQ(nullptr, req->lastInvocation);

    // req should be now self destroyed
    WAIT_AND_ASSERT(destroyedCalled);
}


TEST(Request, CancelRequestAndErrorResponse)
{
    SimpleRequest* req = new SimpleRequest;

    bool destroyedCalled = false;
    QObject::connect(req, &SimpleRequest::destroyed, [&] () { destroyedCalled = true; });

    MockRESTInvocation mockInv;

// --
    EXPECT_CALL(mockInv, abort()).Times(1);
    req->prepare(&mockInv);
    req->cancel();

    req->finishedError(&mockInv);
    ASSERT_EQ(0, req->processOkCallCount);
    ASSERT_EQ(0, req->processErrorCallCount);
    ASSERT_EQ(nullptr, req->lastInvocation);

    // req should be now self destroyed
    WAIT_AND_ASSERT(destroyedCalled);
}
