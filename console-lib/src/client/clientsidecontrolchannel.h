#ifndef CLIENTSIDECONTROLCHANNEL_H
#define CLIENTSIDECONTROLCHANNEL_H

#include <QByteArray>

#include "channelhandler.h"


class ClientSideControlChannel : public ChannelHandler
{
    Q_OBJECT

public:
    ClientSideControlChannel();
    ~ClientSideControlChannel();

    void ping();
    bool isConnectionValid();
    virtual void handleMessage(const QByteArray& msg);

signals:
    void pingReceived();

};

#endif // CLIENTSIDECONTROLCHANNEL_H
