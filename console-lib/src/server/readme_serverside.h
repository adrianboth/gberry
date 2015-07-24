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
 
 #ifndef README_SERVERSIDE_H
#define README_SERVERSIDE_H

// just documentation here
/*
Serverside Communications
=========================

South side refers to application<->comms communication.
North side refers to comms<->mobapps communication

## South Side

CommTcpServer handles low level TCP connections and using specific message
containers in TCP communication that belong to channels.

ConnectionManager handles connection on logical level. There is concept of
application and it takes care of handshaking connection.

ConnectionManager uses ChannelManager (ServerSideChannelManager) to deal with
channels.
 - There is ServerSideControlChannel and ServerSidePlayerChannel.


## North Side

PlayerConnectionManager receives new player connections once they have been
validated. PlayerConnectionManager initiates opening new channels using
ServerSideChannelManager and also closing channels when players disconnect.


*/
#endif // README_SERVERSIDE_H

