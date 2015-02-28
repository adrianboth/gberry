#ifndef CLIENTSIDEPLAYERCHANNEL_H
#define CLIENTSIDEPLAYERCHANNEL_H

#include "playerchannel.h"

class ClientSidePlayerChannel : public PlayerChannel
{
public:
    ClientSidePlayerChannel(int channelId, QObject* parent = 0);
    ~ClientSidePlayerChannel();

};

#endif // CLIENTSIDEPLAYERCHANNEL_H
