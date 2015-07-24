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

#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T17:43:10
#
#-------------------------------------------------

QT       += core network websockets
# TODO: we should split gui and core stuff to separate libs
#QT       -= gui
QT += gui widgets

TARGET = tests
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    test_serverconnectionimpl.cpp \
    test_restinvocationfactoryimpl.cpp \
    utils/testhttpserver.cpp \
    test_systemservices.cpp \
    test_websockets.cpp \
    test_integration_test.cpp \
    test_rest_communication.cpp \
    test_playersession.cpp \
    test_log.cpp \
    utils/testlogmsghandler.cpp \
    test_displayprofile.cpp \
    test_downloadstreaminvocation.cpp \
    test_errors.cpp \
    test_resultmessageformatter.cpp


INCLUDEPATH += ../src ../../gmock/include ../../gtest/include ../../qhttpserver/src

LIBS += -L../../gmock -lgmock -L../src -lgberrylib -L../../qhttpserver/src -lqhttpserver

DEPENDPATH += ../src

PRE_TARGETDEPS += ../src/libgberrylib.a

HEADERS += \
    utils/testhttpserver.h \
    utils/testlogmsghandler.h
