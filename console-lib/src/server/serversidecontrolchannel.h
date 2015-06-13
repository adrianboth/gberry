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

    static const int CHANNEL_ID;

    void pingSouth();

signals:
    void pingSouthReceived();

};

#endif // SERVERSIDECONTROLCHANNEL_H
