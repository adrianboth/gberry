#ifndef CONNECTIONCOMMUNICATION_H
#define CONNECTIONCOMMUNICATION_H

#include <QObject>

#include "client/4qml/connection.h"

class ClientSideControlChannel;

class ConnectionCommunication : public IConnectionCommunication
{
    Q_OBJECT
public:
    explicit ConnectionCommunication(ClientSideControlChannel* controlChannel, QObject *parent = 0);
    ~ConnectionCommunication();

    virtual void sendMessage(const QJsonObject& msg) override;

signals:
    void messageReceived(const QJsonObject msg);

private:
    ClientSideControlChannel* _controlChannel;
};
#endif // CONNECTIONCOMMUNICATION_H
