#!/bin/bash

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

#!/bin/bash

# first parameter is directory to package (typically release folder)
# second parameter is path to zip file

if [ -z $1 ]; then
    echo "ERROR: Please provide path to directory that will be packaged"
    exit 1
fi

if [ -z $2 ]; then
    echo "ERROR: Please provide zip file path"
    exit 1
fi

SRC_DIR=$1
DST_ZIP=$2

echo "Working directory $SRC_DIR ..."
cd $SRC_DIR
if [ $? != 0 ]; then
    echo "ERROR: Failed to change directory to $SRC_DIR"
    exit 1
fi

# TODO: we could have somehow crossplatform way to make packages

echo "Creating zip file $DST_ZIP ..."
zip -r $DST_ZIP .
if [ $? != 0 ]; then
    echo "ERROR: Failed to make zip in $SRC_DIR"
    exit 1
fi

echo "[DONE]"
