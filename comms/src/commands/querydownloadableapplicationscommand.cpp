#include "querydownloadableapplicationscommand.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "server/application/application2json.h"


namespace GBerry
{

class QueryDownloadableApplicationsCommandPrivate
{
public:
    QueryDownloadableApplicationsCommandPrivate() {}

};

QueryDownloadableApplicationsCommand::QueryDownloadableApplicationsCommand() :
    ICommand("QueryDownloadableApplications"),
    _d(new QueryDownloadableApplicationsCommandPrivate)
{
}

QueryDownloadableApplicationsCommand::~QueryDownloadableApplicationsCommand()
{
}

bool QueryDownloadableApplicationsCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no params for request
    Q_UNUSED(json);
    Q_UNUSED(response);

    // TODO: initiate remote query
    //       - once it is ready -> send reply

    /*
    QJsonObject responseJson;
    responseJson["command"] = "QueryLocalApplicationsReply";

    QJsonArray appsList;
    foreach(QSharedPointer<IApplication> app, _priv->apps->applications()) {
        appsList << Application2Json::from(*app);
    }

    responseJson["applications"] = appsList;
    response.set(responseJson);
    */
    return true;
}

} // eon
