#include "querydownloadableapplicationscommand.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QList>

#include "server/serversidecontrolchannel.h"
#include "server/application/application2json.h"
#include "headserverconnection.h"
#include "requests/downloableapplicationsrequest.h"


namespace GBerry
{

class QueryDownloadableApplicationsCommandPrivate
{
public:
    QueryDownloadableApplicationsCommandPrivate(
            HeadServerConnection* headServerConnection_,
            ServerSideControlChannel* controlChannel_) :
        headServerConnection(headServerConnection_),
        controlChannel(controlChannel_) {}

    HeadServerConnection* headServerConnection;
    ServerSideControlChannel* controlChannel;
    QList<DownloadableApplicationsRequest*> ongoingRequests;
};

QueryDownloadableApplicationsCommand::QueryDownloadableApplicationsCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel) :
    ICommand("QueryDownloadableApplications"),
    _d(new QueryDownloadableApplicationsCommandPrivate(headServerConnection, controlChannel))
{
}

QueryDownloadableApplicationsCommand::~QueryDownloadableApplicationsCommand()
{
    // this command is tight to control channel, when channel closes then
    // also this command is destroyed. As there are pointers elsewhere we
    // can't free all request immediately. Canceling will do that when
    // RESTInvocations are ready.

    foreach(auto req, _d->ongoingRequests) {
        req->cancel();
    }
}

bool QueryDownloadableApplicationsCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no params for request
    Q_UNUSED(json);
    Q_UNUSED(response);

    DownloadableApplicationsRequest* request = new DownloadableApplicationsRequest(this);
    _d->ongoingRequests.append(request);
    _d->headServerConnection->makeRequest(request);

    return true;
}

void QueryDownloadableApplicationsCommand::processRequestOkResponse(DownloadableApplicationsRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();

    // TODO: actual handling of application data

    QJsonObject responseJson;
    responseJson["command"] = "QueryDownloadableApplicationsReply";
    responseJson["result"] = "ok";


    /*
    QJsonArray appsList;
    foreach(QSharedPointer<IApplication> app, _priv->apps->applications()) {
        appsList << Application2Json::from(*app);
    }

    responseJson["applications"] = appsList;
    */
    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

void QueryDownloadableApplicationsCommand::processRequestErrorResponse(DownloadableApplicationsRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    request->deleteLater();
    // TODO: some kind of error code (possible localization required later)

    QJsonObject responseJson;
    responseJson["command"] = "QueryDownloadableApplicationsReply";
    responseJson["result"] = "failure";

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

} // eon
