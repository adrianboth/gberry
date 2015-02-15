TARGET = gberrylib
TEMPLATE = lib

include(../global.pri)

QT += core network
QT -= gui

CONFIG += c++11 staticlib

INCLUDEPATH += ../gmock/include ../gtest/include

HEADERS += \
    proxyqnetwork.h \
    mock_proxyqnetwork.h

lib_headers.path    = $${INSTALLDIR}/include/gmock
lib_headers.files   += $$HEADERS

target.path = $${INSTALLDIR}/lib/
INSTALLS += target lib_headers
