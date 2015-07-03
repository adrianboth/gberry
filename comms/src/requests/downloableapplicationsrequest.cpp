#include "downloableapplicationsrequest.h"

#include "commands/querydownloadableapplicationscommand.h"

namespace GBerry {

DownloadableApplicationsRequest::DownloadableApplicationsRequest(
        QueryDownloadableApplicationsCommand* command) :
    _command(command)
{
}

DownloadableApplicationsRequest::~DownloadableApplicationsRequest()
{
}

void DownloadableApplicationsRequest::processPrepare(RESTInvocation *invocation)
{
    // TODO:
}

void DownloadableApplicationsRequest::processOkResponse(RESTInvocation *invocation)
{
    // TODO: parse response (json)
    _command->processRequestOkResponse(this);
}

void DownloadableApplicationsRequest::processErrorResponse(RESTInvocation *invocation)
{
    // TODO: actual action
    _command->processRequestErrorResponse(this);
}


} // eon
