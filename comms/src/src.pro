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

GBERRYLIB_BUILD_DIR=$$PWD/../../../builds/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PWD/../../../gberry-lib/lib/src
includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

QHTTPSERVER_BUILD_DIR=$$PWD/../../../builds/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/qhttpserver/src
QHTTPSERVER_SRC_DIR=$$PWD/../../../gberry-lib/qhttpserver/src
includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
