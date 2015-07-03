#include "request.h"

#include <QTimer>

#include "restinvocation.h"

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

void Request::prepare(RESTInvocation *invocation)
{
    _invocation = invocation;
    processPrepare(invocation);
}

void Request::finishedOk(RESTInvocation *invocation)
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

void Request::finishedError(RESTInvocation *invocation)
{
    if (_active)
        processErrorResponse(invocation);
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
