TEMPLATE = app
TARGET = integrationtests

QT       += core network testlib
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

DEFINES += TESTDATA_ROOT=$$PWD/testdata

include(../project.pri)

SOURCES += main.cpp \
    int_downloadapplicationcommand.cpp \
    int_querydownloadableapplicationscommand.cpp

HEADERS += 

INCLUDEPATH += ../src
LIBS += -L../src -lcomms

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
