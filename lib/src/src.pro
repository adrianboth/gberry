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
    restinvocationfactory.h \
    serverconnection.h \
    serverconnectionimpl.h \
    mocks/mock_restinvocationfactory.h \
    mocks/mock_restinvocation.h \
    restinvocationimpl.h \
    restinvocationfactoryimpl.h \
    systemservices.h \
    realsystemservices.h \
    testutils/gtestrunner.h \
    testutils/signalrecorder.h \
    testutils/waiter.h \
    testutils/qtgtest.h \
    server/websocketserver.h \
    client/websocketclient.h \
    server/playersessionmanager.h \
    server/playersession.h \
    server/websocketconnection.h \
    mocks/mock_playersessionmanager.h \
    server/consolerestserver.h \
    client/consoledevice.h \
    client/consolesessionmanager.h \
    log/ilogmsghandler.h \
    log/log.h \
    log/logcontrol.h \
    log/stdoutlogmsghandler.h \
    ui/displayprofile.h \
    client/applicationstorage.h \
    client/qmlapplication.h \
    client/cppapplication.h \
    client/usermodel.h \
    client/gamemodel.h \
    ui/targetdisplay.h \
    ui/actualdisplay.h \
    ui/display.h \
    applicationsettings.h \
    testutils/util_enablelog.h \
    testutils/util_testdata.h \
    utils/fileutils.h \
    testutils/mock_ienvironmentvariables.h

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

SOURCES += \
    restinvocation.cpp \
    restinvocationfactory.cpp \
    serverconnection.cpp \
    serverconnectionimpl.cpp \
    restinvocationimpl.cpp \
    restinvocationfactoryimpl.cpp \
    realsystemservices.cpp \
    systemservices.cpp \
    testutils/gtestrunner.cpp \
    testutils/qtgtest.cpp \
    server/websocketserver.cpp \
    client/websocketclient.cpp \
    server/playersessionmanager.cpp \
    server/playersession.cpp \
    server/websocketconnection.cpp \
    server/consolerestserver.cpp \
    client/consoledevice.cpp \
    client/consolesessionmanager.cpp \
    log/log.cpp \
    log/logcontrol.cpp \
    log/stdoutlogmsghandler.cpp \
    ui/displayprofile.cpp \
    client/applicationstorage.cpp \
    client/qmlapplication.cpp \
    client/cppapplication.cpp \
    client/usermodel.cpp \
    client/gamemodel.cpp \
    ui/targetdisplay.cpp \
    ui/actualdisplay.cpp \
    applicationsettings.cpp \
    testutils/util_enablelog.cpp

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
