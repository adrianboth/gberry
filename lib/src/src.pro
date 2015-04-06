TARGET = gberrylib
TEMPLATE = lib

#include(../../global.pri)

QT += core network websockets
QT -= gui

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
    client/application.h \
    client/consoledevice.h \
    client/consolesessionmanager.h \
    log/ilogmsghandler.h \
    log/log.h \
    log/logcontrol.h \
    log/stdoutlogmsghandler.h \
    ui/displayprofile.h

#lib_headers.path    = $${INSTALLDIR}/include
#lib_headers.files   += $$HEADERS

#target.path = $${INSTALLDIR}/lib/
#INSTALLS += target lib_headers

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
    client/application.cpp \
    client/consoledevice.cpp \
    client/consolesessionmanager.cpp \
    log/log.cpp \
    log/logcontrol.cpp \
    log/stdoutlogmsghandler.cpp \
    ui/displayprofile.cpp

INCLUDEPATH += ../../qhttpserver/src
LIBS += -L../../qhttpserver/src -lqhttpserver

DISTFILES += \
    ui/qmldir \
    ui/qml/GButton.qml \
    ui/js/DeveloperLog.js \
    ui/qml/GDisplayProfile.qml \
    ui/qml/ConfirmationDialog.qml
