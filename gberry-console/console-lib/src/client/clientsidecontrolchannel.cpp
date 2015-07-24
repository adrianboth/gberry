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
 
 #include "clientsidecontrolchannel.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "client/clientsidechannelpartners.h"
#include "common/messagefactory.h"
using namespace GBerry::Console;

#define LOG_AREA "ClientSideControlChannel"
#include "log/log.h"



class CommandResponseImpl : public ICommandResponse
{
public:
    virtual void set(QJsonObject j) override { json = j; }
    QJsonObject json;
};


class ClientSideControlChannelPrivate
{
public:
    ClientSideControlChannelPrivate() :
        activated(false),
        applicationCode(""),
        partner(nullptr) {}

    bool activated;
    bool connected{false};
    QString applicationCode;
    ClientSideChannelPartner* partner;
    QMap<QString, ICommand*> commands;

    void sendMessage(const QByteArray& msg) {
        if (partner)
            partner->sendMessage(msg);
    }

    bool processJsonMessage(const QJsonObject& json) {
        TRACE("processJsonMessage()")
        if (json.contains("command") && commands.contains(json["command"].toString())) {
            QString cmd = json["command"].toString();
            TRACE("Command found: " << cmd);
            CommandResponseImpl response;
            // commands are not required to have response message
            // 'result' tells if msg was processed
            bool result = commands[cmd]->process(json, response);
            if (result && !response.json.empty()) {
                // TODO: should we automatically add "command_response" ?
                QJsonDocument jsonDoc(response.json);
                partner->sendMessage(jsonDoc.toJson());
            }

            return result;
        }
        // msg didn't match
        TRACE("No match from registered commands");
        return false;
    }
};

const int ClientSideControlChannel::CHANNEL_ID(0);

ClientSideControlChannel::ClientSideControlChannel() :
    _d(new ClientSideControlChannelPrivate)
{
}

ClientSideControlChannel::~ClientSideControlChannel()
{
    TRACE("~ClientSideControlChannel");
}

void ClientSideControlChannel::requestApplicationLaunch(QString appID)
{
    QJsonObject json;
    json["command"] = "LaunchApplication";
    json["application_id"] = appID;
    QJsonDocument jsonDoc(json);
    _d->sendMessage(jsonDoc.toJson());
}

void ClientSideControlChannel::requestApplicationExit()
{
    QJsonObject json;
    json["command"] = "ExitApplication";
    QJsonDocument jsonDoc(json);
    _d->sendMessage(jsonDoc.toJson());
}

void ClientSideControlChannel::setApplicationCode(const QString& code)
{
    _d->applicationCode = code;
}

void ClientSideControlChannel::ping()
{
    DEBUG("Ping requested");
    _d->sendMessage(MessageFactory::createPingCommand(_d->applicationCode));
}

bool ClientSideControlChannel::receiveMessage(const QByteArray& msg)
{
    QJsonParseError errors;

    QJsonDocument doc(QJsonDocument::fromJson(msg, &errors));
    if (errors.error != QJsonParseError::NoError) {
        WARN("Failed to parse json message: error =" << errors.errorString() << ", offset =" << errors.offset << ", message =" << msg);
        return false;
    }

    QJsonObject json(doc.object());
    if (!json.contains("command")) {
        WARN("Not 'command' message -> ignoring");
        return false;
    }
    else if (json["command"] == "Ping")
    {
        emit pingReceived();
        _d->sendMessage(MessageFactory::createPingReply(_d->applicationCode));
        if (!_d->connected) {
            _d->connected = true;
            emit isConnectedChanged();
        }

        return true;
    }
    else if (json["command"] == "PingReply")
    {
        emit pingReceived();
        // but no reply to reply ...
        return true;
    }
    else if (json["command"] == "Activate")
    {
        _d->activated = true;
        emit isActivatedChanged();
        return true;
    }
    else if (json["command"] == "Deactivate")
    {
        _d->activated = false;
        emit isActivatedChanged();
        return true;
    }

    if (_d->processJsonMessage(json))
        return true;

    // not known by us
    return false;
}

void ClientSideControlChannel::sendMessage(const QByteArray& msg)
{
    _d->partner->sendMessage(msg);
}


void ClientSideControlChannel::attachChannelPartner(ClientSideChannelPartner* partner)
{
    _d->partner = partner;
}

void ClientSideControlChannel::detachChannelPartner()
{
    // TODO: not 100% sure whose reponsibility to delete ...
    _d->partner = NULL;
}

int ClientSideControlChannel::channelId() const
{
    return CHANNEL_ID;
}

bool ClientSideControlChannel::isActivated() const
{
    return _d->activated;
}

bool ClientSideControlChannel::isConnected() const
{
    return _d->connected;
}

void ClientSideControlChannel::registerCommand(ICommand *cmd)
{
    if (cmd != nullptr) {
        _d->commands[cmd->id()] = cmd;
    }
}

void ClientSideControlChannel::connectionBroken()
{
    _d->connected = false;
    emit isConnectedChanged();
}

