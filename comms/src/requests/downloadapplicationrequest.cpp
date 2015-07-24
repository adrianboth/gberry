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

    connect(inv, &DownloadStreamInvocation::downloadStarted,
            [&] (DownloadStreamInvocation* inv) { Q_UNUSED(inv); _command->downloadStarted(this); });

    connect(inv, &DownloadStreamInvocation::downloadProgress,
            [&] (DownloadStreamInvocation* inv) { emit _command->downloadProgress(this, inv); });

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

void DownloadApplicationRequest::processErrorResponse(const Result& result, Invocation *invocation)
{
    Q_UNUSED(invocation);
    _command->processRequestErrorResponse(this, result);

    // if request has failed to connection problems then might be that invocation has not been ever created
    if (_invocation != nullptr) {
        _invocation->deleteLater();
        _invocation = nullptr;
    }
}


} // eon
