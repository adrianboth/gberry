#ifndef HEADSERVERSTATUSCOMMAND_H
#define HEADSERVERSTATUSCOMMAND_H

#include <QObject>
#include <QJsonObject>

#include "server/icommand.h"
#include "headserverconnection.h"
using namespace GBerry;

class ServerSideControlChannel;

namespace GBerryComms {

class HeadServerStatusCommand : public QObject, public ICommand
{
    Q_OBJECT

public:
    HeadServerStatusCommand(HeadServerConnection* headServerConnection,
                            ServerSideControlChannel* controlChannel);
    ~HeadServerStatusCommand();

    virtual bool process(const QJsonObject& json, ICommandResponse& response) override;

public slots:
    void onConnected();
    void onDisconnected();

private:
    QJsonObject createResponse(bool currentStatus, bool oldStatus);

    HeadServerConnection* _headServerConnection;
    ServerSideControlChannel* _controlChannel;

};

} // eon


#endif // HEADSERVERSTATUSCOMMAND_H
