message("Setting from global.pri")
INSTALLDIR=/home/tevuore/gberry-deploy

message("INSTALLDIR=$${INSTALLDIR}")

# PWD is relative to this *.pri file
PROJECTS_ROOT_DIR=$$PWD/../..

include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

# Refs to used software. In *.pro these can be use including libs as
# these are not automatically added.
GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src

QHTTPSERVER_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/qhttpserver/src
QHTTPSERVER_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/qhttpserver/src

GMOCK_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/gmock
GMOCK_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/gmock/include

GTEST_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/gtest
GTEST_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/gtest/include
