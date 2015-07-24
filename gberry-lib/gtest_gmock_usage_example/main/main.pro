TARGET = app
TEMPLATE = app

CONFIG += c+11

SOURCES += main.cpp

# TODO: way to get paths
INCLUDEPATH += . ../src

LIBS += -L../src -lapp

# As we are referring to static lib we need to get automatical rebuild if
# static lib changes.
DEPENDPATH += ../src
