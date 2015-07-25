Compiling Platform
==================

NOTE: There is lot to improve in documenting how GBerry platform is built. 
      Please drop a line, and we'll will help you.
 
     
Command Line
------------

Easiest way just to build is use command line build:

 * `development/build_all.sh` builds all possible versions
 * Build result are found from `builds/` and especially from `builds/deploy-*`
   folders.
   

QtCreator
---------

There exists QtCreator project files. The platform consists from following
separate projects:

 * gberry-lib
 * mobile/mobapp
 * gberry-console/console-lib
 * gberry-console/comms
 * gberry-console/mainui
 * gberry-console/waitapp
 
There are example games built as part of the platform:

 * games/gberry-tictactoe
 * games/gberry-react
 

Build Requirements
------------------

Requirements for making a build:

 * Currently Linux only supported (Compiled on Ubuntu)
 * For x86 version Qt5.4 version is required
 * For mobile application build Android NDK and SDK are required. 
 * For Raspberry Pi compiler and system image are needed
 * Ready compiled Qt5.4 need to installed to Raspberry system image. This 
   contains necessary binaries and headers. They just need to be copied.
   
