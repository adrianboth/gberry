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
else
    # TODO: now simple structure, folder and binary with same name
    $CONSOLE_ROOT/apps/$1/$1
fi
