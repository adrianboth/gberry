TEMPLATE = app
TARGET = tests

QT       += core
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

DEFINES += TESTDATA_ROOT=$$PWD/testdata

include(../project.pri)

SOURCES += main.cpp \
    ../src/json/jsonvalidator.cpp \
    ../src/json/jsondefinition.cpp \
    ../src/json/jsondefinitionbuilder.cpp \
    ../src/application.cpp \
    ../src/uiappstatemachine.cpp \
    ../src/applicationcontroller.cpp \
    ../src/launchcontroller.cpp \
    ../src/localapplicationsstorage.cpp \
    ../src/localapplications.cpp \
    ../src/cmdlineparams.cpp \
    test_commchannel.cpp \
    test_uiappstatemachine.cpp \
    test_applicationcontroller.cpp \
    test_localapplicationstorage.cpp \
    test_jsonvalidator.cpp \
    test_cmdlineparams.cpp \
    test_localapplications.cpp \
    test_launchcontroller.cpp

HEADERS += \
    mocks/mock_iapplicationcontroller.h \
    ../src/interfaces/iapplicationsstorage.h \
    ../src/interfaces/iapplicationcontroller.h \
    ../src/interfaces/ilaunchcontroller.h \
    ../src/interfaces/iapplications.h \
    ../src/json/jsonvalidator.h \
    ../src/json/jsondefinition.h \
    ../src/json/jsondefinitionbuilder.h \
    ../src/applicationmeta.h \
    ../src/application.h \
    ../src/applicationcontroller.h \
    ../src/launchcontroller.h \
    ../src/uiappstatemachine.h \
    ../src/uiappstatemachine_private.h \
    ../src/localapplicationsstorage.h \
    ../src/localapplications.h \
    utils/util_controllerproxy.h \
    mocks/mock_iapplicationstorage.h \
    mocks/mock_iapplications.h

INCLUDEPATH += ../src

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})
