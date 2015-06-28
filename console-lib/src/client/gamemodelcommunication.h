#ifndef GAMEMODELCOMMUNICATION_H
#define GAMEMODELCOMMUNICATION_H

#include <QObject>

#include "client/4qml/gamemodel.h"

class ClientSideControlChannel;

class GameModelCommunication : public IGameModelCommunication
{
    Q_OBJECT
public:
    explicit GameModelCommunication(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    ~GameModelCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};

#endif // GAMEMODELCOMMUNICATION_H
