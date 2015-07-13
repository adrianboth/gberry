#include "connectioncommunication.h"

#include <QJsonDocument>

#include "client/clientsidecontrolchannel.h"

#define LOG_AREA "ConnectionCommunication"
#include "log/log.h"


class HeadServerStatusCommand : public ICommand
{
public:
    HeadServerStatusCommand(ConnectionCommunication* comm) :
        ICommand("HeadServerStatus"),
        _comm(comm) {}

    virtual ~HeadServerStatusCommand() {}

    // ICommand interface
    virtual bool process(const QJsonObject& json, ICommandResponse& response) override {
        Q_UNUSED(response);
        DEBUG("process(): HeadServerStatusCommand");
        // parsing will occur in Connection

        // correct signal emit will require right interface
        IConnectionCommunication* icomm = qobject_cast<IConnectionCommunication*>(_comm);
        emit icomm->messageReceived(json);
        return true;
    }

private:
    ConnectionCommunication* _comm;
};



ConnectionCommunication::ConnectionCommunication(
        ClientSideControlChannel* controlChannel,
        QObject *parent) :
    IConnectionCommunication(parent),
    _controlChannel(controlChannel)
{
    controlChannel->registerCommand(new HeadServerStatusCommand(this)); // takes ownership
}

ConnectionCommunication::~ConnectionCommunication()
{
}

void ConnectionCommunication::sendMessage(const QJsonObject& msg)
{
    _controlChannel->sendMessage(QJsonDocument(msg).toJson());
}
