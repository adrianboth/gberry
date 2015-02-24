#ifndef SERVERSIDECONTROLCHANNEL_H
#define SERVERSIDECONTROLCHANNEL_H

#include "channelhandler.h"

#include <QByteArray>


class ServerSideControlChannel : public ChannelHandler
{
    Q_OBJECT

public:
    ServerSideControlChannel();
    ~ServerSideControlChannel();

    void ping();
    bool isConnectionValid();
    virtual void handleMessage(const QByteArray& msg);

signals:
    void pingReceived();

public slots:


};

#endif // SERVERSIDECONTROLCHANNEL_H
