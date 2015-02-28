#ifndef SERVERSIDECHANNELMANAGER_H
#define SERVERSIDECHANNELMANAGER_H

#include <QObject>
#include "channelmanager.h"
#include "playerchannel.h"

// TODO: PlayerMetadata should be moved to common place
#include "client/playersmanager.h"


class ServerSideChannelManager : public ChannelManager
{
public:
    ServerSideChannelManager(QObject* parent = 0);
    ~ServerSideChannelManager();

    int nextFreeChannelId();
    PlayerChannel* openPlayerChannel(PlayerMetadata metadata);

    void reopenPlayerChannels();

public slots:
    void applicationClosed(); // connection towards client closed

private:
    int _channelIdCount;

};

#endif // SERVERSIDECHANNELMANAGER_H
