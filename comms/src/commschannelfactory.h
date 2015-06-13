#ifndef COMMSCHANNELFACTORY_H
#define COMMSCHANNELFACTORY_H

#include "server/channelfactory.h"

namespace GBerry {
namespace Console {
namespace Server {
    class ApplicationRegistry;
}}}

using namespace GBerry::Console::Server;

class ServerSideControlChannel;

namespace GBerryComms {

class CommsCommands;

class CommsChannelFactory : public ChannelFactory
{
public:
    CommsChannelFactory(ApplicationRegistry* registry, CommsCommands* commands);
    ~CommsChannelFactory();

    virtual ServerSideControlChannel* createControlChannel(int connectionId) override;

private:
    ApplicationRegistry* _registry;
    CommsCommands* _commands;
};

} // eon

#endif // COMMSCHANNELFACTORY_H
