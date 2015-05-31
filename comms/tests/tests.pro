TEMPLATE = app
TARGET = tests

QT       += core
# TODO: temporary enabling
#QT       -= gui
QT       += gui widgets

CONFIG   += console c++11
CONFIG   -= app_bundle

include(../project.pri)

SOURCES += main.cpp \
    utils/testtcpclient.cpp \
    test_commchannel.cpp \
    test_uiappstatemachine.cpp \
    ../src/uiappstatemachine.cpp \
    ../src/applicationcontroller.cpp \
    test_applicationcontroller.cpp

HEADERS += \
    utils/testtcpclient.h \
    mocks/mock_iapplicationcontroller.h \
    ../src/applicationmeta.h \
    ../src/iapplicationcontroller.h \
    ../src/applicationcontroller.h \
    ../src/ilaunchcontroller.h \
    ../src/iapplications.h \
    ../src/uiappstatemachine.h \
    ../src/uiappstatemachine_private.h \
    utils/util_controllerproxy.h

INCLUDEPATH += ../src

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeStaticLibrary("gmock", $${GMOCK_SRC_DIR}, $${GMOCK_BUILD_DIR})

includeStaticLibrary("gtest", $${GTEST_SRC_DIR}, $${GTEST_BUILD_DIR})
