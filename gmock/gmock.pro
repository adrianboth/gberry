TARGET = gmock
TEMPLATE = lib

QT += core
QT -= gui

CONFIG += c++11 staticlib

HEADERS += \ 
    include/gmock/internal/gmock-generated-internal-utils.h \
    include/gmock/internal/gmock-internal-utils.h \
    include/gmock/internal/gmock-port.h \
    include/gmock/gmock-actions.h \
    include/gmock/gmock-cardinalities.h \
    include/gmock/gmock-generated-actions.h \
    include/gmock/gmock-generated-function-mockers.h \
    include/gmock/gmock-generated-matchers.h \
    include/gmock/gmock-generated-nice-strict.h \
    include/gmock/gmock-matchers.h \
    include/gmock/gmock-more-actions.h \
    include/gmock/gmock-more-matchers.h \
    include/gmock/gmock-spec-builders.h \
    include/gmock/gmock.h

SOURCES += gtest/src/gtest-all.cc src/gmock-all.cc \
    src/gmock-cardinalities.cc \
    src/gmock-internal-utils.cc \
    src/gmock-matchers.cc \
    src/gmock-spec-builders.cc \
    src/gmock.cc

OTHER_FILES += src/gmock_main.cc

DISTFILES += \
    README

INCLUDEPATH += . include gtest gtest/include

#LIBS += -L../src -lGeekTools -L../lib/qhttpserver/src -lqhttpserver
