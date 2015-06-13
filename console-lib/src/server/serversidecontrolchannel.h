#ifndef SERVERSIDECONTROLCHANNEL_H
#define SERVERSIDECONTROLCHANNEL_H

#include "channel.h"

#include <QByteArray>


class ServerSideControlChannel : public Channel
{
    Q_OBJECT

public:
    ServerSideControlChannel();
    ~ServerSideControlChannel();

    static const int ROOT_CHANNEL_ID;

    void pingSouth();
/*
    // Channel
    virtual bool receiveMessageFromSouth(const QByteArray& msg);
*/
protected:
    virtual bool processJsonMessage(const QJsonObject& json) override;

signals:
    void applicationLaunchRequested(QString appID);
    void applicationExitRequested(); // TODO: how to recognize different apps?

    void pingSouthReceived();

};

#endif // SERVERSIDECONTROLCHANNEL_H
