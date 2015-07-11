#include "downloadstreaminvocation.h"
#include "downloadapplicationrequest.h"
#include "commands/downloadapplicationcommand.h"
#include "invocationfactory.h"



namespace GBerry {

DownloadApplicationRequest::DownloadApplicationRequest(
        DownloadApplicationCommand* command,
        const QString& applicationId,
        const QString& applicationVersion,
        const QString& destinationFilePath) :
    _command(command),
    _applicationId(applicationId),
    _applicationVersion(applicationVersion),
    _destinationFilePath(destinationFilePath)
{
}

DownloadApplicationRequest::~DownloadApplicationRequest()
{
}

QString DownloadApplicationRequest::applicationFullId() const
{
    // TODO: this is now second impl (first in Application) -> only single place
    return _applicationId + "-" + _applicationVersion;
}

QString DownloadApplicationRequest::destinationFilePath() const
{
    return _destinationFilePath;
}

Invocation* DownloadApplicationRequest::processPrepare(InvocationFactory *factory)
{
    DownloadStreamInvocation* inv = factory->newDownloadStreamInvocation();

    // TODO: cleaner way to construct URLs
    inv->defineGetOperation("/application/download/" + _applicationId + "/" + _applicationVersion);
    inv->setOutputFilePath(_destinationFilePath);

    return inv;
}

void DownloadApplicationRequest::processOkResponse(Invocation *invocation)
{
    Q_UNUSED(invocation);

    // TODO: parse response (json) ... who takes care of downloading, i.e. using invocation .. can we even use invocation ... or convert it to something else
    _command->processRequestOkResponse(this);

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    }
}

void DownloadApplicationRequest::processErrorResponse(Error err, Invocation *invocation)
{
    Q_UNUSED(invocation);

    // TODO: actual action
    _command->processRequestErrorResponse(this);

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    }
}


} // eon
