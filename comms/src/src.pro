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

include(../global.pri)


SOURCES += main.cpp \
    gberrymaincontroller.cpp \
    gberryconsoleserver.cpp

HEADERS += \
    gberrymaincontroller.h \
    gberryconsoleserver.h

DEPENDPATH += . ../lib
#INCLUDEPATH += ../lib/qhttpserver/src $${INSTALLDIR}/include
# TODO: pointing directly to sources (easier to manage with QtCreator), howabout project dependencies? and then from command line?
# TODO: gberrylib could have include folder
#INCLUDEPATH += ../lib/qhttpserver/src $${INSTALLDIR}/../workspace/gberry/gberry-lib/lib
# TODO: relative path ends up into binary for connect() errors, static path here doesn't help?
INCLUDEPATH += ../lib/qhttpserver/src

# TODO: some macro to add external libs?
LIBS += -L../lib/qhttpserver/src -lqhttpserver
#-L$${INSTALLDIR}/lib -lgberrylib


GBERRYLIB_BUILD_DIR=$$PWD/../../../builds/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PWD/../../../gberry-lib/lib/src

win32:CONFIG(release, debug|release): LIBS += -L$${GBERRYLIB_BUILD_DIR}/release -lgberrylib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${GBERRYLIB_BUILD_DIR}/debug -lgberrylib
else:unix: LIBS += -L$${GBERRYLIB_BUILD_DIR} -lgberrylib

INCLUDEPATH += $$GBERRYLIB_SRC_DIR
DEPENDPATH += $$GBERRYLIB_SRC_DIR

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/release/libgberrylib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/libgberrylib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/release/gberrylib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/debug/gberrylib.lib
else:unix: PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/libgberrylib.a
