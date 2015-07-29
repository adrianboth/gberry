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
#include "mocks/mock_invocationfactory.h"

#include "testobjects/simplerequest.h"

// basic usage of Request (subclassing)
TEST(Request, OKRequest)
{
    SimpleRequest* req = new SimpleRequest;

    MockRESTInvocation mockInv;
    MockInvocationFactory mockFactory;
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
    MockInvocationFactory mockFactory;
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));

// -- test
    Invocation* inv = req->prepareInvocation(&mockFactory);
    req->finishedError(Result(RequestErrors::INVOCATION_FAILED), &mockInv);

    ASSERT_EQ(1, req->ProcessErrorCallCount);
    ASSERT_EQ(&mockInv, inv);
    ASSERT_EQ(&mockInv, req->LastInvocation);
    ASSERT_TRUE(RequestErrors::INVOCATION_FAILED == req->LastError.errors().first());

    delete req;
}

TEST(Request, CancelRequestAndOKResponse)
{
    SimpleRequest* req = new SimpleRequest;

    bool destroyedCalled = false;
    QObject::connect(req, &SimpleRequest::destroyed, [&] () { destroyedCalled = true; });

    MockInvocationFactory mockFactory;
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
    MockInvocationFactory mockFactory;

// --
    EXPECT_CALL(mockFactory, newRESTInvocation()).Times(1).WillOnce(Return(&mockInv));
    EXPECT_CALL(mockInv, abort()).Times(1);
    req->prepareInvocation(&mockFactory);
    req->cancel();

    req->finishedError(RequestErrors::NO_CONNECTION, &mockInv);
    ASSERT_EQ(0, req->ProcessOkCallCount);
    ASSERT_EQ(0, req->ProcessErrorCallCount);
    ASSERT_EQ(nullptr, req->LastInvocation);

    // req should be now self destroyed
    WAIT_AND_ASSERT(destroyedCalled);
}
