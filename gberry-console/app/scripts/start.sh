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

export LD_LIBRARY_PATH=$CONSOLE_ROOT/lib:$QTDIR/lib
export QML2_IMPORT_PATH=$CONSOLE_ROOT/lib/qml

QTCONF=$THIS_DIR/qt.conf
if [ ! -e $QTCONF ]; then
    echo "[Paths]" > $QTCONF
    echo "Prefix = $QTDIR" >> $QTCONF
fi

export GBERRY_ROOT_PATH=$CONSOLE_ROOT

if [ -z "$1" ]; then
    $THIS_DIR/comms
else
    APP=$1
    shift
    $THIS_DIR/$APP $*
fi
