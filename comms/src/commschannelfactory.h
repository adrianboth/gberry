/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */
 
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
