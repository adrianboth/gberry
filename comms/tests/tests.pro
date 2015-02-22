TEMPLATE = app
TARGET = tests

QT       += core
QT       -= gui

CONFIG   += console c++11
CONFIG   -= app_bundle

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

GTEST_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/gtest
GTEST_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/gtest/include
includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})
