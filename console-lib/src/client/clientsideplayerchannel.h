#ifndef CLIENTSIDEPLAYERCHANNEL_H
#define CLIENTSIDEPLAYERCHANNEL_H

#include "playerchannel.h"
#include "client/playersmanager.h"

class ClientSidePlayerChannel : public PlayerChannel
{
public:
    ClientSidePlayerChannel(int channelId, PlayerMetadata& metadata, QObject* parent = 0);
    ~ClientSidePlayerChannel();

private:
    PlayerMetadata _playerMeta;
};

#endif // CLIENTSIDEPLAYERCHANNEL_H
