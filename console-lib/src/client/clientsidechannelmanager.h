#ifndef CLIENTSIDECHANNELMANAGER_H
#define CLIENTSIDECHANNELMANAGER_H

#include <QObject>
#include "channelmanager.h"
#include "playersmanager.h"
#include "playerchannel.h"


class ClientSideChannelManager : public ChannelManager
{
    Q_OBJECT

public:
    ClientSideChannelManager(QObject* parent = 0);
    ~ClientSideChannelManager();

    // IChannelParent
    virtual void channelCloseReceived(int channelId);

signals:
    void newPlayerChannel(PlayerChannel* channel, PlayerMetadata playerMeta);

public slots:
    virtual void processMessage(int channelId, const QByteArray msg);
};

#endif // CLIENTSIDECHANNELMANAGER_H
