TARGET = consolelib
TEMPLATE = lib

QT       += network
QT       -= gui

CONFIG += c++11

DEFINES += LIB_LIBRARY

SOURCES += messagehandler.cpp \
    commtcpclient.cpp \
    commtcpserver.cpp \
    messagereader.cpp

HEADERS += messagehandler.h\
        lib_global.h \
    commtcpclient.h \
    commtcpserver.h \
    messagereader.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
