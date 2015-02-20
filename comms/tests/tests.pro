#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T09:03:36
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    utils/testtcpclient.cpp \
    test_commchannel.cpp

HEADERS += \
    utils/testtcpclient.h

PROJECTS_ROOT_DIR=$$PWD/../../..
BUILDS_DIR=$${PROJECTS_ROOT_DIR}/builds
include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src
includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

GMOCK_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/gmock
GMOCK_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/gmock/include
includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})
