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
    gberryconsoleserver.cpp

HEADERS += \
    gberryconsoleserver.h

DEPENDPATH += .

# TODO: some macro to add external libs?

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

defineTest(includeStaticLibrary) {
    LIB_NAME=$${1}
    SRC_DIR=$${2}
    BUILD_DIR=$${3}

    message(" -- adding static lib: $${LIB_NAME} $${SRC_DIR} $${BUILD_DIR}")

    win32:CONFIG(release, debug|release): LIBS += -L$${BUILD_DIR}/release -l$${LIB_NAME}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$${BUILD_DIR}/debug -l$${LIB_NAME}
    else:unix: LIBS += -L$${BUILD_DIR} -l$${LIB_NAME}

    INCLUDEPATH += $$SRC_DIR
    DEPENDPATH += $$SRC_DIR

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/lib$${LIB_NAME}.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${GBERRYLIB_BUILD_DIR}/lib$${LIB_NAME}.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/release/$${LIB_NAME}.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $${BUILD_DIR}/debug/$${LIB_NAME}.lib
    else:unix: PRE_TARGETDEPS += $${BUILD_DIR}/lib$${LIB_NAME}.a

    export(LIBS)
    export(INCLUDEPATH)
    export(DEPENDPATH)
    export(PRE_TARGETDEPS)
}

QHTTPSERVER_BUILD_DIR=$$PWD/../../../builds/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/qhttpserver/src
QHTTPSERVER_SRC_DIR=$$PWD/../../../gberry-lib/qhttpserver/src

includeStaticLibrary("qhttpserver", $${QHTTPSERVER_SRC_DIR}, $${QHTTPSERVER_BUILD_DIR})
