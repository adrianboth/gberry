TEMPLATE = app

QT += qml quick network

CONFIG += c++11

SOURCES += main.cpp \
    restquery.cpp \
    appcontroller.cpp \
    serverconnectionmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    restquery.h \
    appcontroller.h \
    serverconnectionmodel.h

# TODO: macro to better handling (should change according architecture)
#  - during development we want to refer to build dirs -> dependencies go better
#  - handle different targets
GBERRYLIB_BUILD_DIR=$$PWD/../../builds/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/src
GBERRYLIB_SRC_DIR=$$PWD/../../gberry-lib/lib/src

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
