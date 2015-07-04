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
    common/messagereader.cpp \
    common/playermeta.cpp \
    common/messagefactory.cpp \
    common/runtimeparameters.cpp \
    client/commtcpclient.cpp \
    client/clientsidecontrolchannel.cpp \
    client/clientsetup.cpp \
    client/clientsideplayerchannel.cpp \
    client/consoleapplication.cpp \
    client/4qml/assets.cpp \
    client/4qml/applicationmanager.cpp \
    client/4qml/gamemodel.cpp \
    client/4qml/playersmanager.cpp \
    client/clientchannelmanager.cpp \
    server/commtcpserver.cpp \
    server/tcpconnection.cpp \
    server/serversidecontrolchannel.cpp \
    server/serversetup.cpp \
    server/connectionmanager.cpp \
    server/serversideplayerchannel.cpp \
    server/channel.cpp \
    server/playerconnectionmanager.cpp \
    server/application/baseapplication.cpp \
    server/application/application2json.cpp \
    server/application/iapplication.cpp \
    server/application/baseapplications.cpp \
    server/applicationregistry.cpp \
    server/connectiongatekeeper.cpp \
    server/serverchannelmanager.cpp \
    server/channelfactory.cpp \
    server/commands/pingcommand.cpp \
    client/applicationparameters.cpp \
    client/applicationmain.cpp \
    client/gamemodelcommunication.cpp \
    client/4qml/connection.cpp \
    client/4qml/downloadablegamesmodel.cpp \
    client/downloadablegamesmodelcommunication.cpp \
    client/4qml/downloadmodel.cpp \
    client/downloadmodelcommunication.cpp

HEADERS +=\
        lib_global.h \
    common/messagereader.h \
    common/playermeta.h \
    common/runtimeparameters.h \
    common/messagefactory.h \
    client/commtcpclient.h \
    client/clientsidecontrolchannel.h \
    client/clientsetup.h \
    client/clientsideplayerchannel.h \
    client/consoleapplication.h \
    client/clientsidechannelpartners.h \
    client/clientchannelmanager.h \
    client/4qml/playersmanager.h \
    client/4qml/assets.h \
    client/4qml/applicationmanager.h \
    client/4qml/gamemodel.h \
    server/connectionmanager.h \
    server/serversideplayerchannel.h \
    server/serversetup.h \
    server/channel.h \
    server/playerconnectionmanager.h \
    server/readme_serverside.h \
    server/icommand.h \
    server/commtcpserver.h \
    server/tcpconnection.h \
    server/serversidecontrolchannel.h \
    server/application/iapplication.h \
    server/application/iapplicationmeta.h \
    server/application/baseapplication.h \
    server/application/iapplications.h \
    server/application/application2json.h \
    server/application/applicationmeta.h \
    server/application/baseapplications.h \
    server/applicationregistry.h \
    server/connectiongatekeeper.h \
    server/channelfactory.h \
    server/channelsouthpartner.h \
    server/serverchannelmanager.h \
    server/commands/pingcommand.h \
    client/applicationparameters.h \
    client/applicationmain.h \
    client/gamemodelcommunication.h \
    client/4qml/connection.h \
    client/4qml/downloadablegamesmodel.h \
    client/downloadablegamesmodelcommunication.h \
    client/4qml/downloadmodel.h \
    client/downloadmodelcommunication.h

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
