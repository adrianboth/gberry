#include "querylocalapplicationscommand.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "server/application/application2json.h"

#define LOG_AREA "QueryLocalApplicationsCommand"
#include "log/log.h"

#include <result.h>
#include <resultmessageformatter.h>
using namespace GBerryLib;

ERRORCLASS(QueryLocalApplicationsCommandError)

class QueryLocalApplicationsCommandErrors {
public:
    static const QueryLocalApplicationsCommandError APPLICATION_NOT_EXIST;
};

// TODO: should we mark this somehow internal error? no localization.
const QueryLocalApplicationsCommandError QueryLocalApplicationsCommandErrors::APPLICATION_NOT_EXIST(3000, "APPLICATION_NOT_EXISTS");


namespace GBerryComms
{

class QueryLocalApplicationsCommandPrivate
{
public:
    QueryLocalApplicationsCommandPrivate(QSharedPointer<IApplications> apps_) :
        apps(apps_) {}

    QSharedPointer<IApplications> apps;
};

QueryLocalApplicationsCommand::QueryLocalApplicationsCommand(QSharedPointer<IApplications> apps) :
    ICommand("QueryLocalApplications")
{
    _priv = new QueryLocalApplicationsCommandPrivate(apps);
}

QueryLocalApplicationsCommand::~QueryLocalApplicationsCommand()
{
    delete _priv;
}

bool QueryLocalApplicationsCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no params for request
    Q_UNUSED(json);

    QJsonObject responseJson;
    responseJson["command"] = "QueryLocalApplicationsReply";
    QJsonArray appsList;

    if (json.contains("application_id")) {
        QString applicationFullId = json["application_id"].toString();
        DEBUG("Checking existance of local app:" << applicationFullId);
        QSharedPointer<IApplication> app = _priv->apps->application(applicationFullId);
        if (!app.isNull()) {
            appsList << Application2Json::from(*app);
        } else {
            responseJson["result"] = "failure";
            responseJson["result_details"] = ResultMessageFormatter(QueryLocalApplicationsCommandErrors::APPLICATION_NOT_EXIST).toJson();
        }
    } else {
        foreach(QSharedPointer<IApplication> app, _priv->apps->applications()) {
            appsList << Application2Json::from(*app);
        }
    }

    responseJson["applications"] = appsList;
    response.set(responseJson);

    return true;
}

} // eon
