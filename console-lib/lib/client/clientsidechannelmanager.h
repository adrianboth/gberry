#ifndef CLIENTSIDECHANNELMANAGER_H
#define CLIENTSIDECHANNELMANAGER_H

#include <QObject>
#include "channelmanager.h"


class ClientSideChannelManager : public ChannelManager
{
public:
    ClientSideChannelManager(QObject* parent = 0);
    ~ClientSideChannelManager();

};

#endif // CLIENTSIDECHANNELMANAGER_H
