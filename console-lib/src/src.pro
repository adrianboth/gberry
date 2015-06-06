TARGET = consolelib
TEMPLATE = lib

QT       += network websockets gui qml quick
# TODO: console lib should be split
#  - common (static)
#  - server (shared), core Qt
#  - client (shared), QtGui
#  but now quick fix
#QT       -= gui

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
    server/playerconnectionmanager.cpp \
    client/consoleapplication.cpp \
    client/assets.cpp \
    client/4qml/applicationmanager.cpp \
    client/4qml/gamemodel.cpp

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
    server/playerconnectionmanager.h \
    client/consoleapplication.h \
    client/assets.h \
    client/4qml/applicationmanager.h \
    server/readme_serverside.h \
    client/4qml/gamemodel.h

qml.files = ui/qml/*
qml.path = $$DEPLOY_DIR/lib/qml/

target.path = $${DEPLOY_DIR}/lib/
INSTALLS += target qml

DISTFILES += \
    ui/qml/GBerryConsole/qmldir \
    ui/qml/GBerryConsole/MessageBoard.qml \
    ui/qml/GBerryConsole/InfoBar.qml \
    ui/qml/GBerryConsole/MainMenuItem.qml \
    ui/qml/GBerryConsole/MainMenu.qml \
    ui/qml/GBerryConsole/AppBoxUI.qml \
    ui/qml/GBerryConsole/js/AppBoxMaster.js \
    ui/qml/GBerryConsole/js/Messages.js
    
includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
