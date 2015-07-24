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
TARGET = integrationtests

QT       += core network testlib
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

DEFINES += TESTDATA_ROOT=$$PWD/testdata

include(../project.pri)

SOURCES += main.cpp \
    int_downloadapplicationcommand.cpp \
    int_querydownloadableapplicationscommand.cpp

HEADERS += 

INCLUDEPATH += ../src
LIBS += -L../src -lcomms

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
