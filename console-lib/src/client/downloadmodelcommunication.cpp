#include "downloadmodelcommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "DownloadModelCommunication"
#include "log/log.h"

namespace GBerry {

class DownloadApplicationStartedReply : public ICommand
{
public:
    DownloadApplicationStartedReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationStartedReply"),
        _comm(comm) {}

    virtual ~DownloadApplicationStartedReply() {}

    // TODO: wondering should somehow separate command and reply in function signatures, as replies won't need response ... typical for client side
    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): DownloadApplicationStartedReply");
        // TODO: validation of json ...

        QString applicationFullId = json["application_id"].toString();
        emit _icomm->downloadStarted(applicationFullId);
        return true;
    }

private:
    IDownloadModelCommunication* _icomm;
};


class DownloadApplicationAbortedReply : public ICommand
{
public:
    DownloadApplicationAbortedReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationAbortedReply"),
        _comm(comm) {}

    virtual ~DownloadApplicationStartedReply() {}

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


class DownloadApplicationFinishedReply : public ICommand
{
public:
    DownloadApplicationFinishedReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationFinishedReply"),
        _comm(comm) {}

    virtual ~DownloadApplicationFinishedReply() {}

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): DownloadApplicationFinishedReply");
        // TODO: validation of json ...

        QString applicationFullId = json["application_id"].toString();
        emit _icomm->downloadFinished(applicationFullId);
        return true;
    }

private:
    IDownloadModelCommunication* _icomm;
};


class DownloadApplicationProgressReply : public ICommand
{
public:
    DownloadApplicationProgressReply(DownloadModelCommunication* comm) :
        ICommand("DownloadApplicationProgressReply"),
        _comm(comm) {}

    virtual ~DownloadApplicationProgressReply() {}

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): DownloadApplicationProgressReply");
        // TODO: validation of json ...

        QString applicationFullId = json["application_id"].toString();
        int progress = json["progress_percentage"].toInt();

        emit _icomm->downloadProgress(applicationFullId, progress);
        return true;
    }

private:
    IDownloadModelCommunication* _icomm;
};


DownloadModelCommunication::DownloadModelCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IDownloadModelCommunication(parent),
    _controlChannel(controlChannel)
{
    // channel takes ownership of commands
    controlChannel->registerCommand(new DownloadApplicationStartedReply(this));
    controlChannel->registerCommand(new DownloadApplicationAbortedReply(this));
    controlChannel->registerCommand(new DownloadApplicationFinishedReply(this));
    controlChannel->registerCommand(new DownloadApplicationProgressReply(this));
}

DownloadModelCommunication::~DownloadModelCommunication()
{
}

void DownloadModelCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}

} // eon
