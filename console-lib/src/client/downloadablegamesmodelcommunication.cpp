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
 
 #include "downloadablegamesmodelcommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "DownloadableGamesModelCommunication"
#include "log/log.h"

namespace GBerry {

class QueryDownloadableApplicationsReply : public ICommand
{
public:
    QueryDownloadableApplicationsReply(DownloadableGamesModelCommunication* comm) :
        ICommand("QueryDownloadableApplicationsReply"),
        _comm(comm) {}

    virtual ~QueryDownloadableApplicationsReply() {}

    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): QueryDownloadableApplicationsReply");
        // parsing will occur in DownloadableGamesModel
        IDownloadableGamesModelCommunication* icomm = qobject_cast<IDownloadableGamesModelCommunication*>(_comm);
        emit icomm->messageReceived(json);
        return true;
    }

private:
    DownloadableGamesModelCommunication* _comm;
};



DownloadableGamesModelCommunication::DownloadableGamesModelCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IDownloadableGamesModelCommunication(parent),
    _controlChannel(controlChannel)
{
    controlChannel->registerCommand(new QueryDownloadableApplicationsReply(this)); // takes ownership
}

DownloadableGamesModelCommunication::~DownloadableGamesModelCommunication()
{
}

void DownloadableGamesModelCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}

} // eon
