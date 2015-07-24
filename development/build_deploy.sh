#!/bin/bash

# $ build_all.sh [clean]

THIS_DIR=`cd $(dirname $0) && pwd`

BUILDS_DIR=$THIS_DIR/../builds

# TODO: x86
# TODO: arm
# TODO: android

die() { echo "$@" 1>&2 ; exit 1; }

function clean_builds {
  
  if [ -e $BUILDS_DIR ]; then
    echo "Deleting under $BUILDS_DIR ..."
    rm -rf $BUILDS_DIR/build-*-Raspberry-Release
    rm -rf $BUILDS_DIR/build-*-Android_for_armeabi_v7a_*-Release
    
  fi
}

if [ "$1" == "clean" ]; then
  clean_builds
fi

if [ ! -e $BUILDS_DIR ]; then
  mkdir -p $BUILDS_DIR
fi

./build_console.sh rpi release || die "Building gberry-console failed!"
./build_mobapp.sh android release || die "Building gberry-mobile failed!"
./build_game.sh rpi release react || die "Building gberry-react-game failed!"
./build_game.sh rpi release tictactoe|| die "Building gberry-tictactoe-game failed!"

echo "[DONE]"