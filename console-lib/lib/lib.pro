#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T16:43:55
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = consolelib
TEMPLATE = lib

DEFINES += LIB_LIBRARY

SOURCES += messagehandler.cpp \
    commtcpclient.cpp \
    commtcpserver.cpp

HEADERS += messagehandler.h\
        lib_global.h \
    commtcpclient.h \
    commtcpserver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
