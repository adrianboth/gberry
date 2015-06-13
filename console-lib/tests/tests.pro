TEMPLATE = app
TARGET = tests

QT       += core network websockets
# TODO: should be separated to gui and non gui libs
#QT       -= gui
QT += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

include(../project.pri)

SOURCES += main.cpp \
    test_commtcp.cpp \
    utils/testtcpserver.cpp \
    test_communication_integration.cpp \
    test_messagereader.cpp \
    utils/testchannel.cpp \
    server/application/test_application2json.cpp \
    server/test_applicationregistry.cpp \
    server/test_serverchannelmanager.cpp \
    utils/testchannelfactory.cpp \
    test_messagefactory.cpp \
    client/test_clientchannelmanager.cpp \
    utils/testplayerchannelnorthpartner.cpp \
    utils/util_messageparser.cpp \
    server/test_connectionmanager.cpp

HEADERS += \
    utils/testtcpserver.h \
    utils/testchannel.h \
    utils/testchannelfactory.h \
    utils/testplayerchannelnorthpartner.h \
    utils/util_messageparser.h \
    utils/testclientsidechannelpartners.h

INCLUDEPATH += ../src
LIBS += -L../src -lconsolelib


includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
