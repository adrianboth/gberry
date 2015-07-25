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
 
 #include "downloadapplicationcommand.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QList>
#include <QProcess>

#include "server/serversidecontrolchannel.h"
#include "server/application/iapplication.h"
#include "server/application/application2json.h"
#include "headserverconnection.h"
#include "localapplicationsstorage.h"
#include "requests/downloadapplicationrequest.h"
#include "downloadableapplicationcache.h"
#include <downloadstreaminvocation.h>
#include <resultmessageformatter.h>

#define LOG_AREA "DownloadApplicationCommand"
#include "log/log.h"

namespace GBerry
{

// TODO: at this point seems little bit useless to have uint codes; how to keep them in sync, better could be category DownloadApplicationCommandErrors
const DownloadApplicationCommandError DownloadApplicationCommandErrors::INTERNAL_ERROR(2000, "INVALID_STATE");


class DownloadApplicationCommandPrivate
{
public:
    DownloadApplicationCommandPrivate(
            HeadServerConnection* headServerConnection_,
            ServerSideControlChannel* controlChannel_,
            DownloadableApplicationCache* cache_,
            LocalApplicationsStorage* applicationsStorage_) :
        headServerConnection(headServerConnection_),
        controlChannel(controlChannel_),
        cache(cache_),
        applicationsStorage(applicationsStorage_) {}

    HeadServerConnection* headServerConnection;
    ServerSideControlChannel* controlChannel;
    DownloadableApplicationCache* cache;
    LocalApplicationsStorage* applicationsStorage;
    QList<DownloadApplicationRequest*> ongoingRequests;
};

DownloadApplicationCommand::DownloadApplicationCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel,
        DownloadableApplicationCache *applicationCache,
        LocalApplicationsStorage* applicationsStorage) :
    ICommand("DownloadApplication"),
    _d(new DownloadApplicationCommandPrivate(headServerConnection,
                                             controlChannel,
                                             applicationCache,
                                             applicationsStorage))
{
}

DownloadApplicationCommand::~DownloadApplicationCommand()
{
    // this command is tight to control channel, when channel closes then
    // also this command is destroyed. As there are pointers elsewhere we
    // can't free all request immediately. Canceling will do that when
    // RESTInvocations are ready.

    foreach(auto req, _d->ongoingRequests) {
        req->cancel();
    }
}

bool DownloadApplicationCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no params for request
    Q_UNUSED(json);
    Q_UNUSED(response);

    // TODO: get the application id we are going to download
    QString applicationFullId = json["application_id"].toString();

    // TODO: where to get user credentials

    // we need full application definition
    if (!_d->cache->hasApplication(applicationFullId)) {
        // TODO: need to query application definition

        ERROR("NOT IMPLEMENTED - In this stage appdef should be in cache!");
        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["application_id"] = applicationFullId;
        responseJson["result"] = "failure";

        Result res(DownloadApplicationCommandErrors::INTERNAL_ERROR);
        res << Result::reasonFromDesc("Application definition not found from cache");
        responseJson["result_details"] = ResultMessageFormatter(res).toJson();

        response.set(responseJson);
        return true;
    }

    QSharedPointer<IApplication> iapp = _d->cache->application(applicationFullId);

    QSharedPointer<Application> newApp = Application::copy(iapp);
    newApp->markState(IApplication::Downloading);

    LocalApplicationsStorage::Result result;
    // TODO: something wrong with QSharedPointer and this ref approach
    if ( !_d->applicationsStorage->addApplication(*newApp.data(), result) ) {
        // TODO: should we clear directory fully? if that was a reason

        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["application_id"] = applicationFullId;
        responseJson["result"] = "failure";

        Result res(DownloadApplicationCommandErrors::INTERNAL_ERROR);
        // TODO: because() better keyword?
        res << Result::reasonFromDesc("Failed to add application to local storage")
            << Result::reasonFromDesc(result.errorString);

        responseJson["result_details"] = ResultMessageFormatter(res).toJson();

        response.set(responseJson);
        return true;
    }

    // everything is ok so far

    // TODO: do we need to downloadengine (yes, to continue downloads)

    // 1) first download to a file (request)
    // 2) then unzip loaded package (when request is ready)

    // TODO: must be better way to handle paths
    QString appZipFilePath = QDir(newApp->meta()->applicationDirPath()).absoluteFilePath("app.zip");

    DownloadApplicationRequest* request = new DownloadApplicationRequest(
                this, newApp->meta()->applicationId(), newApp->meta()->version(), appZipFilePath);

    _d->ongoingRequests.append(request);
    _d->headServerConnection->makeRequest(request);

    return true;
}

void DownloadApplicationCommand::processRequestOkResponse(DownloadApplicationRequest *request)
{
    // this is called when downloading has been succesfully finished

    _d->ongoingRequests.removeOne(request);
    request->deleteLater();

    // unzip downloaded file
    if (!QFile(request->destinationFilePath()).exists()) {
        // something went wrong, because ok has been recorded but no actual zip file
        ERROR("Downloaded zip file" << request->destinationFilePath() << "doesn't exist");
        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["application_id"] = request->applicationFullId();
        responseJson["result"] = "failure";

        Result res(DownloadApplicationCommandErrors::INTERNAL_ERROR);
        // TODO: because() better keyword?
        res << Result::reasonFromDesc("File #{file_path} expected to exist but it does not.")
            << Result::Meta("file_path", request->destinationFilePath());

        responseJson["result_details"] = ResultMessageFormatter(res).toJson();

        _d->controlChannel->sendJsonMessageToSouth(responseJson);
        return;
    }

    // TODO: maybe best approach would be using QuaZip (requires zlib too) but now
    //       quick solution is to call 'unzip'

    UnzipOperation* unzip = new UnzipOperation(
                request->applicationFullId(),
                request->destinationFilePath());

    connect(unzip, &UnzipOperation::finished,
            this, &DownloadApplicationCommand::onUnzipFinished);

    unzip->execute();
}

// because lambda slot trick we can't pass QString by reference
void DownloadApplicationCommand::onUnzipFinished(UnzipOperation* unzipOp)
{
    unzipOp->deleteLater();

    if (unzipOp->exitCode() != 0) {
        // something went wrong
        ERROR("Failed to unzip application package for applicationId:" << unzipOp->applicationFullId());
        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["application_id"] = unzipOp->applicationFullId();
        responseJson["result"] = "failure";

        Result res(DownloadApplicationCommandErrors::INTERNAL_ERROR);
        // TODO: because() better keyword?
        res << Result::reasonFromDesc("Failed to unzip application")
            << Result::Meta("exit_code", QString::number(unzipOp->exitCode()));

        responseJson["result_details"] = ResultMessageFormatter(res).toJson();

        _d->controlChannel->sendJsonMessageToSouth(responseJson);
        return;
    }

    // update state of application
    // (note that extracting might have updated application configuration)
    // (in fact that would mean that we would need to validate it)

    // we need to force rereading
    QSharedPointer<LocalApplications> localApps = _d->applicationsStorage->localApplications();
    // remove temp files
    _d->applicationsStorage->pruneApplication(localApps->application(unzipOp->applicationFullId()));

    // we need to read from the disk (applications get signal but we are not processing signal at this point)
    QSharedPointer<IApplication> iapp = _d->applicationsStorage->localApplications()->application(unzipOp->applicationFullId());
    QSharedPointer<Application> app(qSharedPointerCast<Application>(iapp));
    app->markState(IApplication::Valid);

    LocalApplicationsStorage::Result updateResult; // TODO: as we have global "Result", should this inner class be named differently?
    _d->applicationsStorage->updateApplication(*app.data(), updateResult);

    if (updateResult.hasError()) {
        // something went wrong, because ok has been recorded but no actual zip file
        ERROR("Failed to update application" << unzipOp->applicationFullId());
        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["application_id"] = unzipOp->applicationFullId();
        responseJson["result"] = "failure";

        Result res(DownloadApplicationCommandErrors::INTERNAL_ERROR);
        // TODO: because() better keyword?
        res << Result::reasonFromDesc("Failed to update application")
            << Result::reasonFromDesc(updateResult.errorString);

        responseJson["result_details"] = ResultMessageFormatter(res).toJson();

        _d->controlChannel->sendJsonMessageToSouth(responseJson);
        return;
    }

    // local applications are not directly updated to client side (mainui)
    //  -> finished download will generate a notification that local apps
    //     have been updated

    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "ok";
    responseJson["application_id"] = unzipOp->applicationFullId();

    _d->controlChannel->sendJsonMessageToSouth(responseJson);

}

void DownloadApplicationCommand::processRequestErrorResponse(
        DownloadApplicationRequest *request,
        const Result& result)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();
    // TODO: some kind of error code (possible localization required later)

    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["application_id"] = request->applicationFullId();
    responseJson["result"] = "failure";
    responseJson["result_details"] = ResultMessageFormatter(result).toJson();

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

void DownloadApplicationCommand::downloadStarted(DownloadApplicationRequest *request)
{
    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "status";
    responseJson["status"] = "started";
    responseJson["application_id"] = request->applicationFullId();

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

void DownloadApplicationCommand::downloadProgress(DownloadApplicationRequest *request, DownloadStreamInvocation *inv)
{
    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "status";
    responseJson["status"] = "progress";
    responseJson["progress_percentage"] = inv->progressPercentage();
    responseJson["application_id"] = request->applicationFullId();

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

UnzipOperation::UnzipOperation(const QString &applicationFullId, const QString &zipFilePath) :
    _applicationFullId(applicationFullId),
    _zipFilePath(zipFilePath)
{
}

UnzipOperation::~UnzipOperation()
{
}

QString UnzipOperation::applicationFullId() const
{
    return _applicationFullId;
}

QString UnzipOperation::zipFilePath() const
{
    return _zipFilePath;
}

int UnzipOperation::exitCode() const
{
    return _exitCode;
}

void UnzipOperation::execute()
{
    // TODO: better would built-in zip (library) but Qt doesn't have such thing
    //       and taking Quazip into use takes some work

    QProcess* unzip = new QProcess(this);
    unzip->setProgram("unzip");

    QStringList arguments;
    arguments << "-o" << _zipFilePath;
    unzip->setArguments(arguments);

    QDir appDir(QFileInfo(_zipFilePath).dir());
    DEBUG("Extrating download zip to" << appDir.absolutePath());
    unzip->setWorkingDirectory(appDir.absolutePath());

    QObject::connect(unzip, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(onQProcessFinished(int, QProcess::ExitStatus)));

    unzip->start();
}

void UnzipOperation::onQProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    _exitCode = exitCode;
    emit finished(this);
}

} // eon
