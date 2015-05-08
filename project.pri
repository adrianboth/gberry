# Project specific settings
PROJECTS_ROOT_DIR=$$PWD/..

include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

# for all console apps
DEPLOY_DIR=$$BUILDS_DIR/deploy-console-$${BUILD_NAME}

# Refs to used software. In *.pro these can be use including libs as
# these are not automatically added.
GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src

CONSOLELIB_BUILD_DIR=$$BUILDS_DIR/build-console-lib-$${BUILD_NAME}/src
CONSOLELIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-console/console-lib/src
