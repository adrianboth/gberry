#ifndef CHANNELMANAGERFACTORY_H
#define CHANNELMANAGERFACTORY_H

#include <QString>

#include "playermeta.h"

class ServerSidePlayerChannel;
class ServerSideControlChannel;


namespace GBerry {
namespace Console {
namespace Server {

class ChannelFactory
{
public:
    explicit ChannelFactory();
    ~ChannelFactory();

    virtual ServerSidePlayerChannel* createPlayerChannel(int channelId, PlayerMeta meta);
    virtual ServerSideControlChannel* createControlChannel(int connectionId);

};

}}} // eon

#endif // CHANNELMANAGERFACTORY_H
