Platform Technical Overview
===========================

On high level GBerry consists from following parts:

 * Console: platform on a game console
 * Client: mobile application to control games
 * Server: webstore to download games
 * Game/Application: game running on top of platform.
 
This project is about Console, client and games. Server is a separate project.

GBerry platform is implemented using Qt/C++/QML/Javascript.

Currently (Q3/2015) console can be run on Linux and specifically Raspberry Pi
is targeted.

Client - mobile application called 'mobapp'- can be run on Android devices.


TODO: pic


Software Components
-------------------

On console level the GBerry platform is provided by software component called
'comms'.

To support high level parts there are in software level also

 * gberry-lib: contains common functionality of console and client.
 * console-lib: contains common functionality of comms and games (applications)
 