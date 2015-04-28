TEMPLATE = app
TARGET = react

QT += qml quick network
CONFIG += c++11

include(project.pri)
SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/ui/gberry-lib $$PWD/ui/console-lib

CONFIG(debug) {
    DEFINES += GBERRY_DEBUG_QML_IMPORT_PATH=$$PWD/ui/gberry-lib:$$PWD/ui/console-lib
    DEFINES += GBERRY_ASSETS_FIXED_ROOT_PATH=$$PWD/assets
}

target.path = $${DEPLOY_DIR}/apps/react/
INSTALLS += target

assets.files = assets/*
assets.path = $$DEPLOY_DIR/apps/react/
INSTALLS += assets

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})
