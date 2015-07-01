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
