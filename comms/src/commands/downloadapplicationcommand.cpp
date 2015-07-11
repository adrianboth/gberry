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

#define LOG_AREA "DownloadApplicationCommand"
#include "log/log.h"

namespace GBerry
{

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
        responseJson["result"] = "failure";
        responseJson["application_id"] = applicationFullId;
        responseJson["error_code"] = "UNEXPECTED_ERROR";
        responseJson["error_string"] = "Unexpected situation: application definition not found from cache";
        response.set(responseJson);
        return true;
    }

    QSharedPointer<IApplication> iapp = _d->cache->application(applicationFullId);

    //QSharedPointer<ApplicationMeta> newMeta(new ApplicationMeta(iapp->meta()));
    QSharedPointer<Application> newApp = Application::copy(iapp);
    newApp->markState(IApplication::Downloading);

    LocalApplicationsStorage::Result result;
    // TODO: something wrong with QSharedPointer and this ref approach
    if ( !_d->applicationsStorage->addApplication(*newApp.data(), result) ) {
        // TODO: should we clear directory fully? if that was a reason

        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["result"] = "failure";
        responseJson["application_id"] = applicationFullId;
        responseJson["error_code"] = "FAILED_TO_WRITE_APPLICATION_DEFINITION";
        responseJson["error_string"] = result.errorString;

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
        responseJson["result"] = "failure";
        responseJson["error_code"] = "DOWNLOAD_INTERNAL_ERROR";
        responseJson["error_string"] = QString("Unknown error. File %1 expected to exist but it does not").arg(request->destinationFilePath());
        responseJson["application_id"] = request->applicationFullId();
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
        responseJson["result"] = "failure";
        responseJson["error_code"] = "UNZIP_INTERNAL_ERROR";
        responseJson["error_string"] = QString("Failed to unzip application %1").arg(unzipOp->applicationFullId());
        responseJson["application_id"] = unzipOp->applicationFullId();
        _d->controlChannel->sendJsonMessageToSouth(responseJson);
        return;
    }

    // update state of application
    // (note that extracting might have updated application configuration)
    // (in fact that would mean that we would need to validate it)
    QSharedPointer<IApplication> iapp = _d->applicationsStorage->localApplications()->application(unzipOp->applicationFullId());
    QSharedPointer<Application> app(qSharedPointerCast<Application>(iapp));
    app->markState(IApplication::Valid);

    LocalApplicationsStorage::Result updateResult;
    _d->applicationsStorage->updateApplication(*app.data(), updateResult);

    if (updateResult.hasError()) {
        // something went wrong, because ok has been recorded but no actual zip file
        ERROR("Failed to update application" << unzipOp->applicationFullId());
        QJsonObject responseJson;
        responseJson["command"] = "DownloadApplicationReply";
        responseJson["result"] = "failure";
        responseJson["error_code"] = "DOWNLOAD_INTERNAL_ERROR";
        responseJson["error_string"] = QString("Failed to update application %1: %2").arg(unzipOp->applicationFullId()).arg(updateResult.errorString);
        responseJson["application_id"] = unzipOp->applicationFullId();
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

void DownloadApplicationCommand::processRequestErrorResponse(DownloadApplicationRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();
    // TODO: some kind of error code (possible localization required later)

    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "failure";
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
