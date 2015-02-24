TARGET = consolelib
TEMPLATE = lib

QT       += network
QT       -= gui

CONFIG += c++11

DEFINES += LIB_LIBRARY

SOURCES += \
    client/commtcpclient.cpp \
    server/commtcpserver.cpp \
    messagereader.cpp \
    channelmanager.cpp \
    channelhandler.cpp \
    server/tcpconnection.cpp \
    server/serversidecontrolchannel.cpp \
    client/clientsidecontrolchannel.cpp \
    playercontrolchannel.cpp \
    client/playersmanager.cpp \
    server/serversetup.cpp \
    client/clientsetup.cpp \
    client/clientsidechannelmanager.cpp \
    server/serversidechannelmanager.cpp

HEADERS +=\
        lib_global.h \
    client/commtcpclient.h \
    server/commtcpserver.h \
    messagereader.h \
    channelmanager.h \
    channelhandler.h \
    server/tcpconnection.h \
    server/serversidecontrolchannel.h \
    client/clientsidecontrolchannel.h \
    playercontrolchannel.h \
    client/playersmanager.h \
    server/serversetup.h \
    client/clientsetup.h \
    client/clientsidechannelmanager.h \
    server/serversidechannelmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
