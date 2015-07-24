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


QT       += core
# TODO: remove gui dep
QT       += gui widgets

TARGET = comms
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../src

include(../project.pri)

INCLUDEPATH += ../src
LIBS += -L../src -lcomms

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})

target.path = $${DEPLOY_DIR}/bin/
INSTALLS += target

# this would affect all files, but now just scripts are installed
QMAKE_INSTALL_FILE = install -m 6755
scripts.files = scripts/*
scripts.path = $$DEPLOY_DIR/bin/
INSTALLS += scripts

# *.ini file need to be next to binary
config.files = comms.ini
config.path = $$DEPLOY_DIR/bin/
INSTALLS += config

DISTFILES += \
    scripts/start_app.sh \
    scripts/start.sh \
    comms.ini

