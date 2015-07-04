#include "downloadapplicationcommand.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QList>

#include "server/serversidecontrolchannel.h"
#include "server/application/application2json.h"
#include "headserverconnection.h"
#include "requests/downloableapplicationsrequest.h"


namespace GBerry
{

class DownloadApplicationCommandPrivate
{
public:
    DownloadApplicationCommandPrivate(
            HeadServerConnection* headServerConnection_,
            ServerSideControlChannel* controlChannel_) :
        headServerConnection(headServerConnection_),
        controlChannel(controlChannel_) {}

    HeadServerConnection* headServerConnection;
    ServerSideControlChannel* controlChannel;
    QList<DownloadApplicationRequest*> ongoingRequests;
};

DownloadApplicationCommand::QueryDownloadableApplicationsCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel) :
    ICommand("DownloadApplication"),
    _d(new DownloadApplicationCommandPrivate(headServerConnection, controlChannel))
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
    QString id = json["application_id"].toString();

    // TODO: where to get user credentials

    DownloadApplicationRequest* request = new DownloadApplicationRequest(this, id);
    _d->ongoingRequests.append(request);
    _d->headServerConnection->makeRequest(request);

    return true;
}

void DownloadApplicationCommand::processRequestOkResponse(DownloadApplicationRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();

    // TODO: actual handling streaming data to disk, once that is ready then send response
    //    -- meanwhile progress information can be sent ...

    /*
    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "ok";

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
    */
}

void DownloadApplicationCommand::processRequestErrorResponse(DownloadApplicationRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();
    // TODO: some kind of error code (possible localization required later)

    QJsonObject responseJson;
    responseJson["command"] = "DownloadApplicationReply";
    responseJson["result"] = "failure";
    responseJson["application_id"] = request->applicationId();

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

} // eon
