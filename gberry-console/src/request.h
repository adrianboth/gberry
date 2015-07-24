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
 
 #ifndef REQUEST_H
#define REQUEST_H

#include "invocation.h"
#include "invocationfactory.h"
#include "error.h"
using namespace GBerryLib;


namespace GBerry {

ERRORCLASS(RequestError)

class RequestErrors {
public:
    static const RequestError NO_CONNECTION;
    static const RequestError INVOCATION_FAILED;
    static const RequestError INVALID_JSON_RESPONSE;
};

class Request
{
public:
    explicit Request();
    virtual ~Request();

    //enum Error { ERROR_NO_CONNECTION, ERROR_INVOCATION_FAILED, ERROR_INVALID_JSON_RESPONSE };

    Invocation* prepareInvocation(InvocationFactory* invocationFactory);

    void finishedOk(Invocation* invocation);

    // NOTE: 'invocation' can be null if Error occurs before 'invocation' has been
    //        created.
    void finishedError(Result result, Invocation* invocation);

    // Requests need to be alive as long as RESTInvocation is live. Calling
    // cancel() means that this Request is not longer needed and can be
    // self-destroyed when RESTInvocation finishes.
    virtual void cancel();

protected:
    virtual Invocation* processPrepare(InvocationFactory* factory) = 0;
    virtual void processOkResponse(Invocation* invocation) = 0;
    virtual void processErrorResponse(const Result& error, Invocation* invocation) = 0;

    Invocation* _invocation{nullptr};
    bool _active{true};

};

} // eon

#endif // REQUEST_H
