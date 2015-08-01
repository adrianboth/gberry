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

TARGET = gberrylib
TEMPLATE = lib

#include(../../global.pri)

QT += core network websockets
# for client classes (TODO: should be split to own lib)
QT += qml quick

#QT -= gui
QT += gui widgets

CONFIG += c++11 staticlib

INCLUDEPATH += ../../gmock/include ../../gtest/include

HEADERS += \
    proxyqnetwork.h \
    mock_proxyqnetwork.h \
    restinvocation.h \
    serverconnection.h \
    serverconnectionimpl.h \
    mocks/mock_restinvocation.h \
    restinvocationimpl.h \
    systemservices.h \
    realsystemservices.h \
    testutils/gtestrunner.h \
    testutils/signalrecorder.h \
    testutils/waiter.h \
    testutils/qtgtest.h \
    testutils/jsonutils.h \
    server/websocketserver.h \
    client/websocketclient.h \
    server/playersessionmanager.h \
    server/playersession.h \
    server/websocketconnection.h \
    mocks/mock_playersessionmanager.h \
    server/consolerestserver.h \
    client/consolesessionmanager.h \
    log/ilogmsghandler.h \
    log/log.h \
    log/logcontrol.h \
    log/stdoutlogmsghandler.h \
    ui/displayprofile.h \
    client/applicationstorage.h \
    client/qml/qmlapplication.h \
    client/cppapplication.h \
    client/qml/usermodel.h \
    ui/targetdisplay.h \
    ui/actualdisplay.h \
    ui/display.h \
    applicationsettings.h \
    testutils/util_enablelog.h \
    testutils/util_testdata.h \
    utils/fileutils.h \
    testutils/mock_ienvironmentvariables.h \
    testobjects/stub_systemservices.h \
    utils/qtsignalproxy.h \
    downloadstreaminvocation.h \
    invocation.h \
    downloadstreaminvocationimpl.h \
    httpinvocationdefinition.h \
    invocationfactory.h \
    invocationfactoryimpl.h \
    baseobject.h \
    error.h \
    result.h \
    networkerrorreasons.h \
    resultmessageformatter.h \
    testutils/stringutils.h \
    mocks/mock_invocationfactory.h \
    mocks/mock_downloadstreaminvocation.h \
    client/qml/loginmodel.h \
    client/iapplicationstorage.h \
    testobjects/stubinvocationfactory.h \
    testobjects/stubrestinvocation.h \
    testobjects/stubdownloadstreaminvocation.h

#lib_headers.path    = $${INSTALLDIR}/include
#lib_headers.files   += $$HEADERS

#target.path = $${INSTALLDIR}/lib/
#INSTALLS += target lib_headers

PROJECTS_ROOT_DIR=$$PWD/../../..
include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)
# for all console apps
DEPLOY_DIR=$$BUILDS_DIR/deploy-console-$${BUILD_NAME}

qml.files = ui/qml/*
qml.path = $$DEPLOY_DIR/lib/qml/
INSTALLS += qml

QMAKE_CXXFLAGS += -fPIC

SOURCES += \
    restinvocation.cpp \
    serverconnection.cpp \
    serverconnectionimpl.cpp \
    restinvocationimpl.cpp \
    realsystemservices.cpp \
    systemservices.cpp \
    testutils/gtestrunner.cpp \
    testutils/qtgtest.cpp \
    testutils/jsonutils.cpp \
    server/websocketserver.cpp \
    client/websocketclient.cpp \
    server/playersessionmanager.cpp \
    server/playersession.cpp \
    server/websocketconnection.cpp \
    server/consolerestserver.cpp \
    client/consolesessionmanager.cpp \
    log/log.cpp \
    log/logcontrol.cpp \
    log/stdoutlogmsghandler.cpp \
    ui/displayprofile.cpp \
    client/applicationstorage.cpp \
    client/qml/qmlapplication.cpp \
    client/cppapplication.cpp \
    client/qml/usermodel.cpp \
    ui/targetdisplay.cpp \
    ui/actualdisplay.cpp \
    applicationsettings.cpp \
    testutils/util_enablelog.cpp \
    testobjects/stub_systemservices.cpp \
    utils/qtsignalproxy.cpp \
    downloadstreaminvocation.cpp \
    invocation.cpp \
    downloadstreaminvocationimpl.cpp \
    httpinvocationdefinition.cpp \
    invocationfactory.cpp \
    invocationfactoryimpl.cpp \
    utils/fileutils.cpp \
    baseobject.cpp \
    error.cpp \
    result.cpp \
    networkerrorreasons.cpp \
    resultmessageformatter.cpp \
    testutils/stringutils.cpp \
    mocks/mock_restinvocation.cpp \
    mocks/mock_downloadstreaminvocation.cpp \
    client/qml/loginmodel.cpp \
    testobjects/stubinvocationfactory.cpp \
    testobjects/stubrestinvocation.cpp \
    testobjects/stubdownloadstreaminvocation.cpp

INCLUDEPATH += ../../qhttpserver/src
LIBS += -L../../qhttpserver/src -lqhttpserver

DISTFILES += \
    ui/qml/GBerry/qmldir \
    ui/qml/GBerry/GButton.qml \
    ui/qml/GBerry/js/DeveloperLog.js \
    ui/qml/GBerry/GDisplayProfile.qml \
    ui/qml/GBerry/ConfirmationDialog.qml \
    ui/qml/GBerry/GConfirmationDialog.qml \
    ui/qml/GBerry/GErrorDialog.qml \
    ui/qml/GBerry/GFeedbackDialog.qml \
    ui/qml/GBerry/GSettings.qml
