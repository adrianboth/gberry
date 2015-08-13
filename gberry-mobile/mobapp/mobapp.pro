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

QT += qml quick network websockets widgets svg

CONFIG += c++11

include(project.pri)

SOURCES += main.cpp \
    serverconnectionmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ui/gberry-lib

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serverconnectionmodel.h

include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
