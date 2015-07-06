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
    if (_active && _invocation)
        _invocation->abort();
}

Invocation* Request::prepareInvocation(RESTInvocationFactory *invocationFactory)
{
    _invocation = processPrepare(invocationFactory);
    return _invocation;
}

void Request::finishedOk(Invocation *invocation)
{
    if (_active)
        processOkResponse(invocation);
    else {
        Request* req = this;
        TRACE("Setting up deletion of Request in event loop");
        QTimer::singleShot(0, [=] () { delete req; });
    }
    _active = false;
}

void Request::finishedError(Error err, Invocation* invocation)
{
    if (_active)
        processErrorResponse(err, invocation);
    else {
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
