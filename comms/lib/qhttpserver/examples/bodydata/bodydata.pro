TARGET = bodydata
QMAKE_CXX = ccache g++

QT += network
QT -= gui

CONFIG += debug

INCLUDEPATH += ../../src
LIBS += -L../../src

win32 {
    debug: LIBS += -lqhttpserverd
    else: LIBS += -lqhttpserver
} else {
    LIBS += -lqhttpserver
}

SOURCES = bodydata.cpp
HEADERS = bodydata.h
