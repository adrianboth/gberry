#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`

BUILDS_DIR=$THIS_DIR/../builds

# TODO: x86
# TODO: arm
# TODO: android

die() { echo "$@" 1>&2 ; exit 1; }

function clean_builds {
  
  if [ -e $BUILDS_DIR ]; then
    echo "Deleting everything under $BUILDS_DIR ..."
    rm -rf $BUILDS_DIR
  fi
}

if [ "$1" == "clean" ]; then
  clean_builds
fi

if [ ! -e $BUILDS_DIR ]; then
  mkdir -p $BUILDS_DIR
fi

./build_console.sh both all || die "Building gberry-console failed!"
./build_mobapp.sh both all || die "Building gberry-mobile failed!"

echo "[DONE]"