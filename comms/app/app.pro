
QT       += core
# TODO: remove gui dep
QT       += gui widgets

TARGET = comms
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../src

include(../project.pri)

INCLUDEPATH += ../src
LIBS += -L../src -lcomms

includeSharedLibrary("consolelib", $${CONSOLELIB_SRC_DIR}, $${CONSOLELIB_BUILD_DIR})

target.path = $${DEPLOY_DIR}/bin/
INSTALLS += target

# this would affect all files, but now just scripts are installed
QMAKE_INSTALL_FILE = install -m 6755
scripts.files = scripts/*
scripts.path = $$DEPLOY_DIR/bin/
INSTALLS += scripts

# *.ini file need to be next to binary
config.files = comms.ini
config.path = $$DEPLOY_DIR/bin/
INSTALLS += config

DISTFILES += \
    scripts/start_app.sh \
    scripts/start.sh \
    comms.ini

