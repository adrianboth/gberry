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

export LD_LIBRARY_PATH=$BUILDS_DIR/build-console-lib-Desktop_Qt_5_4_0_GCC_64bit-Debug/src:$BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/src
cd $BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/integration
$BUILDS_DIR/build-comms-Desktop_Qt_5_4_0_GCC_64bit-Debug/integration/integrationtests ||die "comms integration tests failed"

echo "[DONE]"