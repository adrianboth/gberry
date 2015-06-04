TEMPLATE = app
TARGET = comms

QT       += core network
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

include(../project.pri)

SOURCES += main.cpp \
    uiappstatemachine.cpp \
    applicationcontroller.cpp \
    localapplications.cpp \
    localapplicationsstorage.cpp \
    launchcontroller.cpp \
    application.cpp \
    json/jsondefinition.cpp \
    json/jsonvalidator.cpp \
    json/jsondefinitionbuilder.cpp

HEADERS += \
    uiappstatemachine.h \
    applicationcontroller.h \
    uiappstatemachine_private.h \
    localapplications.h \
    applicationmeta.h \
    localapplicationsstorage.h \
    interfaces/iapplicationcontroller.h \
    interfaces/ilaunchcontroller.h \
    interfaces/iapplications.h \
    interfaces/iapplicationsstorage.h \
    launchcontroller.h \
    application.h \
    json/jsondefinition.h \
    json/jsonvalidator.h \
    json/jsondefinitionbuilder.h \
    json/jsonvalidationresult.h \
    interfaces/iapplicationmeta.h

DEPENDPATH += .

linux-g++ {
    CONFIG(debug, debug|release) {
        # build time definition only for desktop and debug
        DEFINES += GBERRY_ROOTPATH=$${DEPLOY_DIR}
    }
}

target.path = $${DEPLOY_DIR}/bin/
INSTALLS += target

# this would affect all files, but now just scripts are installed
QMAKE_INSTALL_FILE = install -m 6755
scripts.files = ../scripts/*
scripts.path = $$DEPLOY_DIR/bin/
INSTALLS += scripts

#filerightsCommand = chmod -R u+x $$DEPLOY_DIR/bin/
#filerights.commands = $$filerightsCommand
#filerights.depends = scripts
#QMAKE_EXTRA_TARGETS += filerights


includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
