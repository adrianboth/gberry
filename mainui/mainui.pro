TEMPLATE = app
TARGET = mainui

QT += qml quick network
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

PROJECTS_ROOT_DIR=$$PWD/../..
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
