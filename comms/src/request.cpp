#include "restinvocation.h"
#include "request.h"

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

    if (_active)
        processErrorResponse(res, invocation);
    else {
        DEBUG("finishedError(): Scheduling deletion");
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
