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
 
 #include "channelfactory.h"

#include "serversideplayerchannel.h"
#include "serversidecontrolchannel.h"
#include "server/commands/pingcommand.h"

namespace GBerry {
namespace Console {
namespace Server {

ChannelFactory::ChannelFactory()
{
}

ChannelFactory::~ChannelFactory()
{
}

ServerSidePlayerChannel* ChannelFactory::createPlayerChannel(int channelId, PlayerMeta meta)
{
    ServerSidePlayerChannel* channel = new ServerSidePlayerChannel(channelId, meta);
    return channel;
}

ServerSideControlChannel* ChannelFactory::createControlChannel(int connectionId)
{
    Q_UNUSED(connectionId);
    ServerSideControlChannel* channel = new ServerSideControlChannel();
    channel->registerCommand(new PingCommand(channel));
    channel->registerCommand(new PingReply(channel));

    return channel;
    // TODO: create generic setup
    // TODO: this function should be inherited to decorate (add things)
}

}}} // eon
