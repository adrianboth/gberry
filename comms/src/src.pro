TEMPLATE = app
TARGET = comms

QT       += core network
QT       -= gui

CONFIG   += console c++11
CONFIG   -= app_bundle

include(../global.pri)

SOURCES += main.cpp \
    gberryconsoleserver.cpp

HEADERS += \
    gberryconsoleserver.h

DEPENDPATH += .

PROJECTS_ROOT_DIR=$$PWD/../../..
BUILDS_DIR=$${PROJECTS_ROOT_DIR}/builds
include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src
includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

QHTTPSERVER_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/qhttpserver/src
QHTTPSERVER_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/qhttpserver/src
includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})

CONSOLELIB_BUILD_DIR=$$BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/src
CONSOLELIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-console/console-lib/src
includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
