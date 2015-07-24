TARGET = runtests
TEMPLATE = app

QT += network

CONFIG += debug c+11

SOURCES += main.cpp \
    test_classundertest.cpp \
    test_mocknetworkaccessmanager.cpp \
    namnuser.cpp \
    template_testclassimpl.cpp

# TODO: way to get paths
INCLUDEPATH += ../src ../../gmock/include ../../gtest/include ../../lib/src

LIBS += -L../src -lapp -L../../gmock -lgmock -L../../lib/src -lgberrylib

DEPENDPATH += ../src

PRE_TARGETDEPS +=  ../src/libapp.a ../../lib/src/libgberrylib.a

HEADERS += \
    mock_iotherservice.h \
    mock_iqsignal.h \
    signalutil.h \
    namnuser.h \
    mock_qnetworkaccessmanager.h
