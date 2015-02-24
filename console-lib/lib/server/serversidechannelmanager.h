#ifndef SERVERSIDECHANNELMANAGER_H
#define SERVERSIDECHANNELMANAGER_H

#include <QObject>
#include "channelmanager.h"


class ServerSideChannelManager : public ChannelManager
{
public:
    ServerSideChannelManager(QObject* parent = 0);
    ~ServerSideChannelManager();

    int nextFreeChannelId();

public slots:
    void applicationClosed(); // connection towards client closed

private:
    int _channelIdCount;

};

#endif // SERVERSIDECHANNELMANAGER_H
