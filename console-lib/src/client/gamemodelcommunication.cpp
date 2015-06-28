#include "gamemodelcommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

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
        // parsing will occur in GameModel
        emit _comm->messageReceived(json);
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
