TEMPLATE = app
TARGET = tictactoe

QT += qml quick network widgets
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

target.path = $${DEPLOY_DIR}/apps/$$TARGET/
INSTALLS += target

assets.files = assets/*
assets.path = $$DEPLOY_DIR/apps/$$TARGET/
INSTALLS += assets

appcfg.files = tictactoe_appcfg.json
appcfg.path = $$DEPLOY_DIR/apps/$$TARGET/
INSTALLS += appcfg

includeStaticLibrary("gberrylib", $${GBERRYLIB_SRC_DIR}, $${GBERRYLIB_BUILD_DIR})

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})

DISTFILES += \
    tictactoe_appcfg.json \
    makepkg.sh

# this would affect all files, but now just scripts are installed
#QMAKE_INSTALL_FILE = install -m 6755
#scripts.files = ../scripts/*
#scripts.path = $$DEPLOY_DIR/bin/
#INSTALLS += scripts

srcdir = $${DEPLOY_DIR}/apps/$$TARGET/
## TODO: version
zipfile = $$OUT_PWD/$${TARGET}.zip
scriptCmd = $$PWD/makepkg.sh \"$$srcdir\" \"$$zipfile\"

makepkg.commands = $$scriptCmd
first.depends = $(first) makepkg
#export(first.depends)
#export(makepkg.commands)
QMAKE_EXTRA_TARGETS += first makepkg
