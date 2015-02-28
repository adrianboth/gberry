#ifndef CLIENTSIDECONTROLCHANNEL_H
#define CLIENTSIDECONTROLCHANNEL_H

#include <QByteArray>

#include "controlchannel.h"


class ClientSideControlChannel : public ControlChannel
{
    Q_OBJECT

public:
    ClientSideControlChannel();
    ~ClientSideControlChannel();

};

#endif // CLIENTSIDECONTROLCHANNEL_H
