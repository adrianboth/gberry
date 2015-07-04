#include "downloadapplicationrequest.h"

#include "commands/downloadapplicationcommand.h"

namespace GBerry {

DownloadApplicationRequest::DownloadApplicationRequest(
        DownloadableApplicationCommand* command) :
    _command(command)
{
}

DownloadApplicationRequest::~DownloadApplicationRequest()
{
}

void DownloadApplicationRequest::processPrepare(RESTInvocation *invocation)
{
    // TODO:
}

void DownloadApplicationRequest::processOkResponse(RESTInvocation *invocation)
{
    // TODO: parse response (json) ... who takes care of downloading, i.e. using invocation .. can we even use invocation ... or convert it to something else
    _command->processRequestOkResponse(this);
}

void DownloadApplicationRequest::processErrorResponse(RESTInvocation *invocation)
{
    // TODO: actual action
    _command->processRequestErrorResponse(this);
}


} // eon
