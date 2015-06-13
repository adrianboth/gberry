#ifndef TESTCHANNELFACTORY_H
#define TESTCHANNELFACTORY_H

#include <QObject>

#include "server/channelfactory.h"

using namespace GBerry::Console::Server;

class TestChannelFactory : public QObject, public ChannelFactory
{
    Q_OBJECT
public:
    TestChannelFactory();
    ~TestChannelFactory();

    int lastChannelId;
    PlayerMeta lastPlayerMeta;
    ServerSidePlayerChannel* lastPlayerChannel;

    int lastConnectionId;
    ServerSideControlChannel* lastControlChannel;

    int controlChannelClosedCount;

    // from ChannelFactory
    virtual ServerSidePlayerChannel* createPlayerChannel(int channelId, PlayerMeta meta) override;
    virtual ServerSideControlChannel* createControlChannel(int connectionId) override;

};

#endif // TESTCHANNELFACTORY_H
