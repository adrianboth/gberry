#include "restinvocation.h"
#include "request.h"

#include <QTimer>

#define LOG_AREA "Request"
#include "log/log.h"

namespace GBerry {

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

void Request::finishedError(Error err, Invocation* invocation)
{
    DEBUG("finishedError()");

    if (_active)
        processErrorResponse(err, invocation);
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
