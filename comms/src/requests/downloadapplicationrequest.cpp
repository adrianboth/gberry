#include "downloadstreaminvocation.h"
#include "downloadapplicationrequest.h"
#include "commands/downloadapplicationcommand.h"
#include "restinvocationfactory.h"



namespace GBerry {

DownloadApplicationRequest::DownloadApplicationRequest(
        DownloadApplicationCommand* command, const QString& applicationFullId) :
    _command(command),
    _applicationFullId(applicationFullId)
{
}

DownloadApplicationRequest::~DownloadApplicationRequest()
{
}

QString DownloadApplicationRequest::applicationFullId() const
{
    return _applicationFullId;
}

Invocation* DownloadApplicationRequest::processPrepare(RESTInvocationFactory *factory)
{
    DownloadStreamInvocation* inv = factory->newDownloadStreamInvocation();

    // TODO: xxx
    inv->defineGetOperation("/foobar");

    return inv;
}

void DownloadApplicationRequest::processOkResponse(Invocation *invocation)
{
    // TODO: parse response (json) ... who takes care of downloading, i.e. using invocation .. can we even use invocation ... or convert it to something else
    _command->processRequestOkResponse(this);

    invocation->deleteLater();
    _invocation = nullptr;
}

void DownloadApplicationRequest::processErrorResponse(Error err, Invocation *invocation)
{
    // TODO: actual action
    _command->processRequestErrorResponse(this);
    invocation->deleteLater();
    _invocation = nullptr;
}


} // eon
