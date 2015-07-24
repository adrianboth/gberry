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
 
 #include "gamemodelcommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "GameModelCommunication"
#include "log/log.h"


class QueryLocalApplicationsReply : public ICommand
{
public:
    QueryLocalApplicationsReply(GameModelCommunication* comm) :
        ICommand("QueryLocalApplicationsReply"),
        _comm(comm) {}

    virtual ~QueryLocalApplicationsReply() {}

    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): QueryLocalApplicationsReply");
        // parsing will occur in GameModel
        IGameModelCommunication* icomm = qobject_cast<IGameModelCommunication*>(_comm);
        emit icomm->messageReceived(json);
        return true;
    }

private:
    GameModelCommunication* _comm;
};



GameModelCommunication::GameModelCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IGameModelCommunication(parent),
    _controlChannel(controlChannel)
{
    controlChannel->registerCommand(new QueryLocalApplicationsReply(this)); // takes ownership
}

GameModelCommunication::~GameModelCommunication()
{
}

void GameModelCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}
