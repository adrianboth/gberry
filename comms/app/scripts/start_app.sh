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
THIS_DIR=`cd $(dirname $0) && pwd`
CONSOLE_ROOT=`cd $THIS_DIR/.. && pwd`

QTDIR=`cd $THIS_DIR/../lib/Qt/5.4.0 && pwd`

export LD_LIBRARY_PATH=$CONSOLE_ROOT/lib:$QTDIR/lib:$CONSOLE_ROOT/apps/$1
export QML2_IMPORT_PATH=$CONSOLE_ROOT/lib/qml

QTCONF=$CONSOLE_ROOT/apps/$1/qt.conf
if [ ! -e $QTCONF ]; then
    echo "[Paths]" > $QTCONF
    echo "Prefix = $QTDIR" >> $QTCONF
fi

if [ -z "$1" ]; then
    echo "ERROR: Please provide application name!"
    exit 1
fi

APPDIR=$1

if [ -z "$2" ]; then
   EXE=$1
else
   EXE=$2
fi


# TODO: now simple structure, folder and binary with same name
$CONSOLE_ROOT/apps/$APPDIR/$EXE
