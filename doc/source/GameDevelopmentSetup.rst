Game Development Setup
======================

This page focuses for setting up a development setup for a new game.

TODO: These instructions are under work.

GBerry platform project contains few example games in `games/` folder that can
be used as base for a new game.

TODO: 
 * creating symlinks
 * packaging
 
Referring to Platform QML
-------------------------

For console applications both `console-lib` and `gberry-lib` provide provide
QML components. For the final deployment setup these QML components are copied
to `lib/`. In other words applications do not need to include those into their
binary.

During development time QML libraries are found via symbolic links. In 
application there is `ui/` folder that contains symbolic links to `gberry-lib`
and `console-lib`.

The `development/create_application_qml_symlinks.sh` can be used to create
needed symbolic links. Call it with a path to a game project directory.

 
Development Tips
----------------

## Standalone Application

To speedup development write-launch-test cycle it may be beneficial to support
playing the game play by mouse (clicks). This way you don't need all the time
use `mobapp` to control the game. 

