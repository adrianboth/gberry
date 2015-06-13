#include "serversidecontrolchannel.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "messagefactory.h"

using namespace GBerry::Console;

#define LOG_AREA "ServerSideControlChannel"
#include "log/log.h"


const int ServerSideControlChannel::CHANNEL_ID = 0;


ServerSideControlChannel::ServerSideControlChannel() :
    Channel(CHANNEL_ID)
{
}

ServerSideControlChannel::~ServerSideControlChannel()
{
    TRACE("~ServerSideControlChannel");
}

void ServerSideControlChannel::pingSouth()
{
    DEBUG("Ping south requested");
    sendMessageToSouth(MessageFactory::createPingCommand());
}
