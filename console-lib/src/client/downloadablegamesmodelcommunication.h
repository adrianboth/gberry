#ifndef DOWNLOADABLEGAMESMODELCOMMUNICATION_H
#define DOWNLOADABLEGAMESMODELCOMMUNICATION_H

#include <QObject>

#include "client/4qml/downloadablegamesmodel.h"

class ClientSideControlChannel;

namespace GBerry {

class DownloadableGamesModelCommunication : public IDownloadableGamesModelCommunication
{
    Q_OBJECT
public:
    explicit DownloadableGamesModelCommunication(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    virtual ~DownloadableGamesModelCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};

} // eon

#endif // DOWNLOADABLEGAMESMODELCOMMUNICATION_H
