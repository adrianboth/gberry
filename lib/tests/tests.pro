#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T17:43:10
#
#-------------------------------------------------

QT       += core network websockets
# TODO: we should split gui and core stuff to separate libs
#QT       -= gui
QT += gui widgets

TARGET = tests
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    test_serverconnectionimpl.cpp \
    test_restinvocationfactoryimpl.cpp \
    utils/testhttpserver.cpp \
    test_systemservices.cpp \
    test_websockets.cpp \
    test_integration_test.cpp \
    test_rest_communication.cpp \
    test_playersession.cpp \
    test_log.cpp \
    utils/testlogmsghandler.cpp \
    test_displayprofile.cpp \
    test_downloadstreaminvocation.cpp

INCLUDEPATH += ../src ../../gmock/include ../../gtest/include ../../qhttpserver/src

LIBS += -L../../gmock -lgmock -L../src -lgberrylib -L../../qhttpserver/src -lqhttpserver

DEPENDPATH += ../src

PRE_TARGETDEPS += ../src/libgberrylib.a

HEADERS += \
    utils/testhttpserver.h \
    utils/testlogmsghandler.h
