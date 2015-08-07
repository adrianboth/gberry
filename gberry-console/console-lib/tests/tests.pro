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
TARGET = tests

QT       += core network websockets qml
# TODO: should be separated to gui and non gui libs
#QT       -= gui
QT += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

include(../project.pri)

SOURCES += main.cpp \
    test_commtcp.cpp \
    test_communication_integration.cpp \
    utils/testtcpserver.cpp \
    utils/testchannel.cpp \
    utils/testplayerchannelnorthpartner.cpp \
    utils/util_messageparser.cpp \
    utils/testchannelfactory.cpp \
    common/test_messagereader.cpp \
    common/test_messagefactory.cpp \
    server/application/test_application2json.cpp \
    server/test_applicationregistry.cpp \
    server/test_serverchannelmanager.cpp \
    server/test_connectionmanager.cpp \
    client/test_clientchannelmanager.cpp \
    common/test_runtimeparameters.cpp \
    client/4qml/test_gamemodel.cpp \
    client/test_gamemodelcommunication.cpp \
    server/application/test_baseapplications.cpp \
    server/test_serversideplayerchannel.cpp \
    utils/stubconsoleapplication.cpp \
    client/test_applicationversionnumber.cpp
    
HEADERS += \
    utils/testtcpserver.h \
    utils/testchannel.h \
    utils/testchannelfactory.h \
    utils/testplayerchannelnorthpartner.h \
    utils/util_messageparser.h \
    utils/testclientsidechannelpartners.h \
    utils/stubconsoleapplication.h

INCLUDEPATH += ../src
LIBS += -L../src -lconsolelib


includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
