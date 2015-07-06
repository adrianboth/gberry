#include "invocation.h"
#include "downloableapplicationsrequest.h"
#include "commands/querydownloadableapplicationscommand.h"
#include "restinvocationimpl.h"

namespace GBerry {

DownloadableApplicationsRequest::DownloadableApplicationsRequest(
        QueryDownloadableApplicationsCommand* command) :
    _command(command)
{
}

DownloadableApplicationsRequest::~DownloadableApplicationsRequest()
{
}

Invocation* DownloadableApplicationsRequest::processPrepare(RESTInvocationFactory *factory)
{
    RESTInvocation* inv = factory->newRESTInvocation();
    // TODO: xxx
    inv->defineGetOperation("/foobar");

    return inv;
}

void DownloadableApplicationsRequest::processOkResponse(Invocation *invocation)
{
    // TODO: parse response (json)
    _command->processRequestOkResponse(this);

    invocation->deleteLater();
    _invocation = nullptr;
}

void DownloadableApplicationsRequest::processErrorResponse(Request::Error error, Invocation *invocation)
{
    // TODO: actual action
    _command->processRequestErrorResponse(this);
    invocation->deleteLater();
    _invocation = nullptr;
}


} // eon
