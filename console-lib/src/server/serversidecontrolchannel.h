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

};

#endif // SERVERSIDECONTROLCHANNEL_H
