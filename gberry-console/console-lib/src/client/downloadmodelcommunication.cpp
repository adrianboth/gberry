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
 
#include "downloadmodelcommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "DownloadModelCommunication"
#include "log/log.h"

namespace GBerryApplication {

class DownloadApplicationReply : public ICommand
{
public:
    DownloadApplicationReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationReply"),
        _icomm(comm) {}

    virtual ~DownloadApplicationReply() {}

    // TODO: wondering should somehow separate command and reply in function signatures, as replies won't need response ... typical for client side
    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): DownloadApplicationReply");
        // TODO: validation of json ...
        QString applicationFullId = json["application_id"].toString();

        if (json["result"].toString() == "ok") {
            emit _icomm->downloadFinished(applicationFullId);

        } else if (json["result"].toString() == "failure") {
            // TODO: how to show error reason
            emit _icomm->downloadAborted(applicationFullId);

        } else if (json["result"].toString() == "status") {

            if (json["status"].toString() == "started") {
                emit _icomm->downloadStarted(applicationFullId);

            } else if (json["status"].toString() == "progress") {
                int progress = json["progress_percentage"].toInt();
                emit _icomm->downloadProgress(applicationFullId, progress);

            } else {
                ERROR("Unknown type of DownloadApplicationReply 'status':" << json["status"].toString());
                return false;
            }
        } else {
            ERROR("Unknown type of DownloadApplicationReply:" << json["result"].toString());
            return false;
        }

        return true;
    }

private:
    IDownloadModelCommunication* _icomm;
};

// TOD: how abort would go (not error)
/*
class DownloadApplicationAbortedReply : public ICommand
{
public:
    DownloadApplicationAbortedReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationAbortedReply"),
        _icomm(comm) {}

    virtual ~DownloadApplicationAbortedReply() {}

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): DownloadApplicationAbortedReply");
        // TODO: validation of json ...

        QString applicationFullId = json["application_id"].toString();
        emit _icomm->downloadAborted(applicationFullId);
        return true;
    }

private:
    IDownloadModelCommunication* _icomm;
};
*/


DownloadModelCommunication::DownloadModelCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IDownloadModelCommunication(parent),
    _controlChannel(controlChannel)
{
    // channel takes ownership of commands
    controlChannel->registerCommand(new DownloadApplicationReply(this));
}

DownloadModelCommunication::~DownloadModelCommunication()
{
}

void DownloadModelCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}

} // eon
