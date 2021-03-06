*************
GBerry Client
*************

Overview
========

GBerry client is a general purpose client for GBerry platform. The client is
implemented as Qt/C++ mobile application.

 * Players connect to a GBerry console
 * Players control a console
 * Players control a game
 
The client has couple of built-in control mechanisms (e.g. basic control and
question dialog) and rest of game UI is transfered from a GBerry console.


Supported Devices
=================

Currently support exists only for **Android** devices, although with help of Qt it
will be future relative easy to support also other platforms.

Design goal is that GBerry client can be used on variety of screen sizes.

For development purposes the client is possible the run on a computer. 
Currently on Linux build is supported.

Development Environment
=======================

Development setup supports:

 * QtCreator
 * Command line build (separate scripts found from `gberry-development`)
 
The client build expects to find `gberry-lib` from same level as root of this 
repository (`gberry-mobile`).

 * Static library and headers are looked for
 * .qml and .js files are looked via symbolic link

Env Vars
--------

There are couple of environment variables to support development on 
development machine.

**MOBAPP_ID** 
 * Used to define identity for MobApp running instance. Identity is used
   to define where application settings are saved.
   
**MOBAPP_AUTO_LOGIN**
 * If '1' or 'true' then MobApp tries to make login automatically using
   last saved information. Boosts run & test cycle.
 
 

