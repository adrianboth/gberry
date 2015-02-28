#ifndef SERVERSIDEPLAYERCHANNEL_H
#define SERVERSIDEPLAYERCHANNEL_H

#include "playerchannel.h"
// TODO: metadata not from good location
#include "client/playersmanager.h"

class ServerSidePlayerChannel : public PlayerChannel
{
public:
    ServerSidePlayerChannel(int channelId,
                            PlayerMetadata metadata,
                            QObject* qparent = 0);
    ~ServerSidePlayerChannel();

    void open();

    // Channel
    virtual void reopen();

private:
    PlayerMetadata _playerMeta;
};

#endif // SERVERSIDEPLAYERCHANNEL_H
