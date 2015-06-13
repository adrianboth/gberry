#include "exitapplicationcommand.h"

namespace GBerryComms {

ExitApplicationCommand::ExitApplicationCommand(const QString& applicationId):
    ICommand("ExitApplication"),
    _applicationId(applicationId)
{
}

ExitApplicationCommand::~ExitApplicationCommand()
{
}

bool ExitApplicationCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    // no response message
    Q_UNUSED(response);
    Q_UNUSED(json);
    emit exitApplicationRequested(_applicationId);
    return true;
}

} // eon
