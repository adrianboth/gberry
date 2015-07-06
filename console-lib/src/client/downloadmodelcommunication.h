#ifndef DOWNLOADMODELCOMMUNICATION_H
#define DOWNLOADMODELCOMMUNICATION_H

// TODO: could communication classes be moved to common subfolder ...

#include <QObject>
#include <QJsonObject>

#include "client/4qml/downloadmodel.h"

class ClientSideControlChannel;

namespace GBerry {

class DownloadModelCommunication : public IDownloadModelCommunication
{
    Q_OBJECT
public:
    explicit DownloadModelCommunication(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    virtual ~DownloadModelCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};

} // eon


#endif // DOWNLOADMODELCOMMUNICATION_H
