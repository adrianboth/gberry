#include "headserverstatuscommand.h"

#include "server/serversidecontrolchannel.h"

namespace GBerryComms {

HeadServerStatusCommand::HeadServerStatusCommand(
        HeadServerConnection* headServerConnection,
        ServerSideControlChannel* controlChannel):
    ICommand("HeadServerStatus"),
    _headServerConnection(headServerConnection),
    _controlChannel(controlChannel)
{
    connect(headServerConnection, &HeadServerConnection::connected,
            this, &HeadServerStatusCommand::onConnected);

    connect(headServerConnection, &HeadServerConnection::disconnected,
            this, &HeadServerStatusCommand::onDisconnected);
}

HeadServerStatusCommand::~HeadServerStatusCommand()
{
}

bool HeadServerStatusCommand::process(const QJsonObject &json, ICommandResponse& response)
{
    Q_UNUSED(json);

    // same as there is no change, just query
    response.set(createResponse(
                     _headServerConnection->isConnected(),
                     _headServerConnection->isConnected()));
    return true;
}

void HeadServerStatusCommand::onConnected()
{
    _controlChannel->sendJsonMessageToSouth(createResponse(true, false));
}

void HeadServerStatusCommand::onDisconnected()
{
    _controlChannel->sendJsonMessageToSouth(createResponse(false, true));
}

QJsonObject HeadServerStatusCommand::createResponse(bool currentStatus, bool oldStatus)
{
    QJsonObject responseJson;
    responseJson["command"] = "HeadServerStatus"; // TODO: doesn't match to general message formats
    responseJson["status"] =  currentStatus ? "connected" : "disconnected";
    responseJson["old_status"] = oldStatus ? "connected" : "disconnected";

    return responseJson;
}

} // eon
