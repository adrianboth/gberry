##
# This file is part of GBerry.

# Copyright 2015 Tero Vuorela
#
# GBerry is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with GBerry. If not, see <http://www.gnu.org/licenses/>.
##

# Project specific settings
PROJECTS_ROOT_DIR=$$PWD/../..

include($${PROJECTS_ROOT_DIR}/gberry-lib/qmake/functions.pri)

# for all console apps
DEPLOY_DIR=$$BUILDS_DIR/deploy-console-$${BUILD_NAME}

# Refs to used software. In *.pro these can be use including libs as
# these are not automatically added.
GBERRYLIB_BUILD_DIR=$$BUILDS_DIR/build-gberry-lib-$${BUILD_NAME}/lib/src
GBERRYLIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-lib/lib/src

CONSOLELIB_BUILD_DIR=$$BUILDS_DIR/build-console-lib-$${BUILD_NAME}/src
CONSOLELIB_SRC_DIR=$$PROJECTS_ROOT_DIR/gberry-console/console-lib/src
