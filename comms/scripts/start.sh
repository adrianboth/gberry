#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
CONSOLE_ROOT=`cd $THIS_DIR/.. && pwd`

export LD_LIBRARY_PATH=$CONSOLE_ROOT/lib:$CONSOLE_ROOT/lib/Qt/5.4.0/lib

QTDIR=`cd $THIS_DIR/../lib/Qt/5.4.0 && pwd`
QTCONF=$THIS_DIR/qt.conf
if [ ! -e $QTCONF ]; then
    echo "[Paths]" > $QTCONF
    echo "Prefix = $QTDIR" >> $QTCONF
fi

if [ -z "$1" ]; then
    $THIS_DIR/comms
else
    $THIS_DIR/$1
fi
