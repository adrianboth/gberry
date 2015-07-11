TEMPLATE = app
TARGET = tests

QT       += core network testlib
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

DEFINES += TESTDATA_ROOT=$$PWD/testdata

include(../project.pri)

SOURCES += main.cpp \
    test_uiappstatemachine.cpp \
    test_applicationcontroller.cpp \
    test_localapplicationstorage.cpp \
    test_jsonvalidator.cpp \
    test_localapplications.cpp \
    test_launchcontroller.cpp \
    commands/test_querylocalapplicationscommand.cpp \
    test_commsparameters.cpp \
    test_headserverconnection.cpp \
    test_request.cpp \
    commands/test_querydownloadableapplicationscommand.cpp \
    integration/int_downloadapplicationcommand.cpp \
    integration/int_querydownloadableapplicationscommand.cpp \
    test_applicationconfigreaderwriter.cpp

HEADERS += \
    mocks/mock_iapplicationcontroller.h \
    utils/util_controllerproxy.h \
    mocks/mock_iapplicationstorage.h \
    mocks/mock_iapplications.h \
    test_request.h

INCLUDEPATH += ../src
LIBS += -L../src -lcomms

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
