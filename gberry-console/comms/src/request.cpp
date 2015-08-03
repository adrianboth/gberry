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
 
#include "restinvocation.h"
#include "request.h"
#include "resultmessageformatter.h"

#include <QTimer>

#define LOG_AREA "Request"
#include "log/log.h"

namespace GBerry {

const RequestError RequestErrors::NO_CONNECTION(10000, "NO_CONNECTION", QT_TRANSLATE_NOOP("Errors", "TXT_No working connection to server #{address}"));
const RequestError RequestErrors::INVOCATION_FAILED(10001, "INVOCATION_FAILED", QT_TRANSLATE_NOOP("Errors", "TXT_Invocation for server failed"));
const RequestError RequestErrors::INVALID_JSON_RESPONSE(1002, "INVALID_JSON_RESPONSE", QT_TRANSLATE_NOOP("Errors", "TXT_Received invalid response for invocation"));

Request::Request()
{
}

Request::~Request()
{
    DEBUG("~Request()");
    if (_active && _invocation) {
        DEBUG("~Request(): Aborting invocation");
        _invocation->abort();
    }
}

Invocation* Request::prepareInvocation(InvocationFactory *invocationFactory)
{
    _invocation = processPrepare(invocationFactory);
    return _invocation;
}

void Request::finishedOk(Invocation *invocation)
{
    DEBUG("finishedOk()");
    if (_active)
        processOkResponse(invocation);
    else {
        DEBUG("finishedOk(): Scheduling deletion");
        Request* req = this;
        TRACE("Setting up deletion of Request in event loop");
        QTimer::singleShot(0, [=] () { delete req; });
    }
    _active = false;
}

void Request::finishedError(Result res, Invocation* invocation)
{
    DEBUG("finishedError()");
    fillInErrorDetails(res);

    if (_active)
        processErrorResponse(res, invocation);
    else {
        DEBUG("finishedError(): Scheduling deletion. Result was:" << ResultMessageFormatter(res).createDeveloperMessage());
        Request* req = this;
        TRACE("Setting up deletion of Request in event loop");
        QTimer::singleShot(0, [=] () { delete req; });
    }
    _active = false;
}

void Request::cancel()
{
    _active = false;
    if (_invocation)
        _invocation->abort();
}

} // eon
