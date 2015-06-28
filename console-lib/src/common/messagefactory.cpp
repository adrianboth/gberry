#include "messagefactory.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "server/application/application2json.h"


namespace {

    QJsonObject createSimpleCommandJson(QString cmdName)
    {
        QJsonObject json;
        json["command"] = cmdName;
        return json;
    }
    QByteArray createSimpleCommand(QString cmdName)
    {
        QJsonDocument jsonDoc(createSimpleCommandJson(cmdName));
        return jsonDoc.toJson();
    }

    QJsonObject createSimpleReplyJson(QString cmdName)
    {
        QJsonObject json;
        json["command"] = cmdName;
        return json;
    }
    QByteArray createSimpleReply(QString cmdName)
    {
        QJsonDocument jsonDoc(createSimpleReplyJson(cmdName));
        return jsonDoc.toJson();
    }


    QByteArray PING_COMMAND(createSimpleCommand("Ping"));
    QJsonObject PING_COMMAND_JSON(createSimpleCommandJson("Ping"));

    QByteArray PING_REPLY(createSimpleReply("PingReply"));
    QJsonObject PING_REPLY_JSON(createSimpleReplyJson("PingReply"));
}

namespace GBerry {
namespace Console {

MessageFactory::MessageFactory()
{
}

MessageFactory::~MessageFactory()
{
}

QByteArray MessageFactory::createPingCommand(const QString& applicationIdCode)
{
    QJsonObject json = createPingCommandJson();
    if (!applicationIdCode.isEmpty()) {
        json["application_code"] = applicationIdCode;
    }
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson();
}

QByteArray MessageFactory::createPingReply(const QString& applicationIdCode)
{
    QJsonObject json = createPingReplyJson();
    if (!applicationIdCode.isEmpty()) {
        json["application_code"] = applicationIdCode;
    }
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson();
}

QByteArray MessageFactory::createPingCommand()
{
    return PING_COMMAND;
}

QByteArray MessageFactory::createPingReply()
{
    return PING_REPLY;
}

QJsonObject MessageFactory::createPingCommandJson()
{
    return PING_COMMAND_JSON;
}

QJsonObject MessageFactory::createPingReplyJson()
{
    return PING_REPLY_JSON;
}

QByteArray MessageFactory::createOpenChannelCommand(PlayerMeta playerMeta)
{
    QJsonObject json = createSimpleCommandJson("OpenChannel");
    json["player_metadata"] = playerMeta.json();

    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson();
}

QByteArray MessageFactory::createOpenChannelReply()
{
    return createSimpleCommand("OpenChannelAccepted");
}

QByteArray MessageFactory::createCloseChannelCommand()
{
    return createSimpleCommand("CloseChannel");
}

QByteArray MessageFactory::createPlayerMessage(QJsonObject playerData)
{
    QJsonObject json;
    json["command"] = "PlayerMessage";
    json["data"] = QString(QJsonDocument(playerData).toJson());
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson();
}

QByteArray MessageFactory::createPlayerMessage(QString str)
{
    QJsonObject json;
    json["command"] = "PlayerMessage";
    json["data"] = str;
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson();
}

QJsonObject MessageFactory::createQueryLocalApplicationsReply(QSharedPointer<IApplications> apps)
{
    QJsonObject responseJson;
    responseJson["command"] = "QueryLocalApplicationsReply";

    QJsonArray appsList;
    foreach(QSharedPointer<IApplication> app, apps->applications()) {
        appsList << Application2Json::from(*app);
    }

    responseJson["applications"] = appsList;
    return responseJson;
}

}}
