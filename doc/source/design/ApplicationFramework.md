Application Framework
=====================

`gberry-lib` provides common code to be shared between
 * console
   * comms
   * console applications
 
 * client
 
 
== UI Components

`ui/` contains UI components.

They expect from QML runtime context:
 * There is ApplicationSettings with id `gsettings`.
   * E.g. global logging level
 
 * There instantiated `GDisplayProfile` with id `gdisplay`.
   * UI component scaling
 
GBerry client includes there into qrc file via symbolic link.

Console applications refer directly to folder
 * Runtime QML import path needs to be ok
 * Files need to copied to deploy directories
 