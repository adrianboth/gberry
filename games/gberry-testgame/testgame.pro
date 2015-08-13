##
# This file is part of GBerry.

# Copyright 2015 Tero Vuorela
#
# GBerry is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with GBerry. If not, see <http://www.gnu.org/licenses/>.
##

TEMPLATE = app
TARGET = testgame

QT += qml quick network widgets
CONFIG += c++11

include(project.pri)
SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/ui/gberry-lib $$PWD/ui/console-lib

CONFIG(debug) {
    DEFINES += GBERRY_DEBUG_QML_IMPORT_PATH=$$PWD/ui/gberry-lib:$$PWD/ui/console-lib
    DEFINES += GBERRY_ASSETS_FIXED_ROOT_PATH=$$PWD/assets
    DEFINES += GBERRY_FIXED_ROOT_PATH=$$PWD
}

target.path = $${DEPLOY_DIR}/apps/$$TARGET/
INSTALLS += target

assets.files = assets/*
assets.path = $$DEPLOY_DIR/apps/$$TARGET/
INSTALLS += assets

appcfg.files = $${TARGET}_appcfg.json testgame_catalog.png VERSION_HISTORY.txt
appcfg.path = $$DEPLOY_DIR/apps/$$TARGET/
INSTALLS += appcfg

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})

DISTFILES += \
    $${TARGET}_appcfg.json \
    VERSION_HISTORY.txt \
    dev.txt

srcdir = $${DEPLOY_DIR}/apps/$$TARGET/
## TODO: version
zipfile = $$OUT_PWD/$${TARGET}.zip
scriptCmd = $$PWD/makepkg.sh \"$$srcdir\" \"$$zipfile\"

# this 'path' is mandatory but doesn't have meaning
makepkg.path = .
makepkg.extra = $$scriptCmd
INSTALLS += makepkg
