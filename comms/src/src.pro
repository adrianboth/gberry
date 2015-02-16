#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T20:03:17
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = comms
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    gberrymaincontroller.cpp \
    gberryconsoleserver.cpp

HEADERS += \
    gberrymaincontroller.h \
    gberryconsoleserver.h

DEPENDPATH += . ../lib
INCLUDEPATH += . ../lib/qhttpserver/src

LIBS += -L../lib/qhttpserver/src -lqhttpserver
