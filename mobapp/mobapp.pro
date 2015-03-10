TEMPLATE = app

QT += qml quick network websockets

CONFIG += c++11

SOURCES += main.cpp \
    serverconnectionmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serverconnectionmodel.h

PROJECTS_ROOT_DIR=$$PWD/../..
BUILDS_DIR=$${PROJECTS_ROOT_DIR}/builds
include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src
includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})
