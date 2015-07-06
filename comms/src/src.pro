TEMPLATE = lib
TARGET = comms

QT       += core network
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

DEFINES += LIB_LIBRARY

include(../project.pri)

SOURCES += \
    uiappstatemachine.cpp \
    applicationcontroller.cpp \
    localapplications.cpp \
    localapplicationsstorage.cpp \
    launchcontroller.cpp \
    application.cpp \
    json/jsondefinition.cpp \
    json/jsonvalidator.cpp \
    json/jsondefinitionbuilder.cpp \
    commschannelfactory.cpp \
    commands/launchapplicationcommand.cpp \
    commands/querylocalapplicationscommand.cpp \
    commands/commscommands.cpp \
    commands/exitapplicationcommand.cpp \
    comms.cpp \
    commsparameters.cpp \
    headserverconnection.cpp \
    request.cpp \
    requests/downloableapplicationsrequest.cpp \
    commands/querydownloadableapplicationscommand.cpp \
    commands/downloadapplicationcommand.cpp \
    requests/downloadapplicationrequest.cpp \
    downloadengine.cpp

HEADERS += \
    uiappstatemachine.h \
    applicationcontroller.h \
    uiappstatemachine_private.h \
    localapplications.h \
    localapplicationsstorage.h \
    interfaces/iapplicationcontroller.h \
    interfaces/ilaunchcontroller.h \
    interfaces/iapplicationsstorage.h \
    launchcontroller.h \
    application.h \
    json/jsondefinition.h \
    json/jsonvalidator.h \
    json/jsondefinitionbuilder.h \
    json/jsonvalidationresult.h \
    commschannelfactory.h \
    commands/launchapplicationcommand.h \
    commands/querylocalapplicationscommand.h \
    commands/commscommands.h \
    commands/exitapplicationcommand.h \
    comms.h \
    commsparameters.h \
    headserverconnection.h \
    request.h \
    requests/downloableapplicationsrequest.h \
    commands/querydownloadableapplicationscommand.h \
    commands/downloadapplicationcommand.h \
    requests/downloadapplicationrequest.h \
    downloadengine.h

DEPENDPATH += .

linux-g++ {
    CONFIG(debug, debug|release) {
        # build time definition only for desktop and debug
        DEFINES += GBERRY_ROOT_PATH=$${DEPLOY_DIR}
    }
}

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
