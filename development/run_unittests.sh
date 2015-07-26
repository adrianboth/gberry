#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`
BUILDS_DIR=$THIS_DIR/../builds

if [ "$1" == "help" ]; then
  echo "USAGE: $0 rpi|desktop|both(default)] [debug|release|all(default)]"
fi

# -- Build

function die { echo "$@" 1>&2 ; exit 1; }

# running only x86 debug tests

cd $BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/tests/
$BUILDS_DIR/build-gberry-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/lib/tests/tests || die "gberry-lib tests failed"

echo "-------------------------------------------------------------------------"

export LD_LIBRARY_PATH=$BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/src
cd $BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/tests
$BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/tests/tests || die "console-lib tests failed"

echo "-------------------------------------------------------------------------"

export LD_LIBRARY_PATH=$BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/src:$BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/src
cd $BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/tests
$BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/tests/tests || die "comms tests failed"

echo "[DONE]"