#include "querydownloadableapplicationscommand.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QList>

#include "server/serversidecontrolchannel.h"
#include "server/application/application2json.h"
#include "headserverconnection.h"
#include "requests/downloableapplicationsrequest.h"
#include "downloadableapplicationcache.h"

namespace GBerry
{

class QueryDownloadableApplicationsCommandPrivate
{
public:
    QueryDownloadableApplicationsCommandPrivate(
            HeadServerConnection* headServerConnection_,
            ServerSideControlChannel* controlChannel_,
            DownloadableApplicationCache* cache_) :
        headServerConnection(headServerConnection_),
        controlChannel(controlChannel_),
        cache(cache_) {}

    HeadServerConnection* headServerConnection;
    ServerSideControlChannel* controlChannel;
    DownloadableApplicationCache* cache;
    QList<DownloadableApplicationsRequest*> ongoingRequests;
};

QueryDownloadableApplicationsCommand::QueryDownloadableApplicationsCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel,
        DownloadableApplicationCache *applicationCache) :
    ICommand("QueryDownloadableApplications"),
    _d(new QueryDownloadableApplicationsCommandPrivate(headServerConnection,
                                                       controlChannel,
                                                       applicationCache))
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

    QJsonArray appsList;
    QList<QSharedPointer<Application>> apps = request->receivedApplications();
    foreach(QSharedPointer<Application> app, apps) {
        QSharedPointer<IApplication> iapp(qSharedPointerCast<IApplication>(app));
        _d->cache->cacheApplication(app);
        appsList << Application2Json::from(*iapp.data());
    }

    responseJson["applications"] = appsList;

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

void QueryDownloadableApplicationsCommand::processRequestErrorResponse(DownloadableApplicationsRequest *request)
{
    _d->ongoingRequests.removeOne(request);
    //request->deleteLater();
    // TODO: some kind of error code (possible localization required later)

    QJsonObject responseJson;
    responseJson["command"] = "QueryDownloadableApplicationsReply";
    responseJson["result"] = "failure";

    _d->controlChannel->sendJsonMessageToSouth(responseJson);
}

} // eon
