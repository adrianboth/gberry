#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
CONSOLE_ROOT=`cd $THIS_DIR/.. && pwd`

export LD_LIBRARY_PATH=$CONSOLE_ROOT/lib

if [ -z "$1" ]; then
    $THIS_DIR/comms
else
    $THIS_DIR/$1
fi
