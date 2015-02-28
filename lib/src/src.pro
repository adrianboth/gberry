TARGET = gberrylib
TEMPLATE = lib

include(../../global.pri)

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
    client/websocketclient.h

lib_headers.path    = $${INSTALLDIR}/include
lib_headers.files   += $$HEADERS

target.path = $${INSTALLDIR}/lib/
INSTALLS += target lib_headers

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
    client/websocketclient.cpp
