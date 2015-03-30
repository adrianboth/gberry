TEMPLATE = app

QT += qml quick network websockets

CONFIG += c++11

include(project.pri)

SOURCES += main.cpp \
    serverconnectionmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serverconnectionmodel.h

include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})
