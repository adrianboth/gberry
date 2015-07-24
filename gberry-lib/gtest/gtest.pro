TARGET = gtest
TEMPLATE = lib

include(../global.pri)

QT += core
QT -= gui

CONFIG += c++11 staticlib

#HEADERS += 

INCLUDEPATH += . include

#LIBS += -L../src -lGeekTools -L../lib/qhttpserver/src -lqhttpserver

SOURCES += src/gtest-all.cc

#    src/gtest-death-test.cc \
#    src/gtest-filepath.cc \
#    src/gtest-port.cc \
#    src/gtest-printers.cc \
#    src/gtest-test-part.cc \
#    src/gtest-typed-test.cc \
#    src/gtest.cc \


OTHER_FILES += samples/sample1.cc \
    samples/sample10_unittest.cc \
    samples/sample1_unittest.cc \
    samples/sample2.cc \
    samples/sample2_unittest.cc \
    samples/sample3_unittest.cc \
    samples/sample4.cc \
    samples/sample4_unittest.cc \
    samples/sample5_unittest.cc \
    samples/sample6_unittest.cc \
    samples/sample7_unittest.cc \
    samples/sample8_unittest.cc \
    samples/sample9_unittest.cc \
    src/gtest_main.cc

DISTFILES += \
    README


HEADERS += \
    include/gtest/internal/gtest-death-test-internal.h \
    include/gtest/internal/gtest-filepath.h \
    include/gtest/internal/gtest-internal.h \
    include/gtest/internal/gtest-linked_ptr.h \
    include/gtest/internal/gtest-param-util-generated.h \
    include/gtest/internal/gtest-param-util.h \
    include/gtest/internal/gtest-port.h \
    include/gtest/internal/gtest-string.h \
    include/gtest/internal/gtest-tuple.h \
    include/gtest/internal/gtest-type-util.h \
    include/gtest/gtest-death-test.h \
    include/gtest/gtest-message.h \
    include/gtest/gtest-param-test.h \
    include/gtest/gtest-printers.h \
    include/gtest/gtest-spi.h \
    include/gtest/gtest-test-part.h \
    include/gtest/gtest-typed-test.h \
    include/gtest/gtest.h \
    include/gtest/gtest_pred_impl.h \
    include/gtest/gtest_prod.h \
    samples/prime_tables.h \
    samples/sample1.h \
    samples/sample2.h \
    samples/sample3-inl.h \
    samples/sample4.h \
    src/gtest-internal-inl.h


gtest_headers.path    = $${INSTALLDIR}/include/gtest
gtest_headers.files   += $$HEADERS

target.path = $${INSTALLDIR}/lib/
INSTALLS += target gtest_headers
