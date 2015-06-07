#include "querylocalapplicationscommand.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "server/application/application2json.h"


namespace GBerry {
    namespace Console {
        namespace Server {


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

    QJsonObject responseJson;
    responseJson["response"] = "QueryLocalApplications";

    QJsonArray appsList;
    foreach(QSharedPointer<IApplication> app, _priv->apps->applications()) {
        appsList << Application2Json::from(*app);
    }

    responseJson["applications"] = appsList;
    response.set(responseJson);

    return true;
}

}}} // eon
