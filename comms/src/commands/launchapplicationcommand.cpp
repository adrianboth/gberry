#include "launchapplicationcommand.h"

#include <QJsonDocument>
#include <QJsonArray>

#include "server/application/application2json.h"

#define LOG_AREA "LaunchApplicationCommand"
#include "log/log.h"

namespace GBerryComms {


class LaunchApplicationCommandPrivate
{
public:
    LaunchApplicationCommandPrivate(QSharedPointer<IApplications> apps_) :
        apps(apps_) {}

    QSharedPointer<IApplications> apps;
};

LaunchApplicationCommand::LaunchApplicationCommand(QSharedPointer<IApplications> apps) :
    ICommand("LaunchApplication")
{
    _priv = new LaunchApplicationCommandPrivate(apps);
}

LaunchApplicationCommand::~LaunchApplicationCommand()
{
    delete _priv;
}

bool LaunchApplicationCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no response message
    Q_UNUSED(response);

    if (json.contains("application_id")) {
        emit launchApplicationRequested(json["application_id"].toString());
        return true;
    } else {
        WARN("Invalid LaunchApplicationCommand: application_id missing");
        return false;
    }
}

} // eon
