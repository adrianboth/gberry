#include "pingcommand.h"

#include "server/serversidecontrolchannel.h"
#include "common/messagefactory.h"

using namespace GBerry::Console;

namespace GBerry {
namespace Console {
namespace Server {

namespace {
    static const char* PING_ID = "Ping";
}

PingCommand::PingCommand(ServerSideControlChannel* manager) :
    ICommand(PING_ID),
    _manager(manager)
{
    Q_ASSERT(manager);
}

PingCommand::~PingCommand()
{
}

bool PingCommand::process(const QJsonObject& json, ICommandResponse& response)
{
    if (json.contains("command") && json["command"] == PING_ID)
    {
        emit _manager->pingSouthReceived();
        response.set(MessageFactory::createPingReplyJson());
        return true;
    }

    return false;
}

// ---
namespace {
    static const char* PINGREPLY_ID = "PingReply";
}

PingReply::PingReply(ServerSideControlChannel* manager) :
    ICommand(PINGREPLY_ID),
    _manager(manager)
{
    Q_ASSERT(manager);
}

PingReply::~PingReply()
{
}

bool PingReply::process(const QJsonObject& json, ICommandResponse& response)
{
    Q_UNUSED(response);
    if (json.contains("command") && json["command"] == PINGREPLY_ID)
    {
        emit _manager->pingSouthReceived();
        // but no reply to reply ...
        return true;
    }

    return false;
}

}}} //eon
