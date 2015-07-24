TEMPLATE=lib
TARGET=app

# Creating this as static lib to enable easy linking to mobile apps
CONFIG += c++11 staticlib

SOURCES += classundertest.cpp \
    otherservice.cpp

HEADERS += classundertest.h \
    otherservice.h \
    iotherservice.h \
    iqsignal.h

INCLUDEPATH += .

#QMAKE_CXXFLAGS += -fPIC
QMAKE_CXXFLAGS += -Wall
