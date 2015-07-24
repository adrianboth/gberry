/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
