#ifndef SERVERSIDECONTROLCHANNEL_H
#define SERVERSIDECONTROLCHANNEL_H

#include "controlchannel.h"

#include <QByteArray>


class ServerSideControlChannel : public ControlChannel
{
    Q_OBJECT

public:
    ServerSideControlChannel();
    ~ServerSideControlChannel();

    //virtual bool receiveMessage(const QByteArray msg) override;
    virtual bool processJsonMessage(const QJsonObject& json) override;

signals:
    void applicationLaunchRequested(QString appID);
    void applicationExitRequested(); // TODO: how to recognize different apps?
};

#endif // SERVERSIDECONTROLCHANNEL_H
