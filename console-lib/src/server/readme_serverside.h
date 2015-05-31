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

