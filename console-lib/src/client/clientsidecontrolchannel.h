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

    void requestApplicationLaunch(QString appID);
    void requestApplicationExit();

};

#endif // CLIENTSIDECONTROLCHANNEL_H
