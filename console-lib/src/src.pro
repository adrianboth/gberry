TARGET = consolelib
TEMPLATE = lib

QT       += network websockets
QT       -= gui

CONFIG += c++11

include(../project.pri)

DEFINES += LIB_LIBRARY

SOURCES += \
    client/commtcpclient.cpp \
    server/commtcpserver.cpp \
    messagereader.cpp \
    channelmanager.cpp \
    server/tcpconnection.cpp \
    server/serversidecontrolchannel.cpp \
    client/clientsidecontrolchannel.cpp \
    client/playersmanager.cpp \
    server/serversetup.cpp \
    client/clientsetup.cpp \
    client/clientsidechannelmanager.cpp \
    server/serversidechannelmanager.cpp \
    server/connectionmanager.cpp \
    server/serversideplayerchannel.cpp \
    client/clientsideplayerchannel.cpp \
    playerchannel.cpp \
    controlchannel.cpp \
    channel.cpp \
    server/playerconnectionmanager.cpp

HEADERS +=\
        lib_global.h \
    client/commtcpclient.h \
    server/commtcpserver.h \
    messagereader.h \
    channelmanager.h \
    server/tcpconnection.h \
    server/serversidecontrolchannel.h \
    client/clientsidecontrolchannel.h \
    client/playersmanager.h \
    server/serversetup.h \
    client/clientsetup.h \
    client/clientsidechannelmanager.h \
    server/serversidechannelmanager.h \
    server/connectionmanager.h \
    server/serversideplayerchannel.h \
    client/clientsideplayerchannel.h \
    playerchannel.h \
    controlchannel.h \
    channel.h \
    ichannelparent.h \
    server/playerconnectionmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
