#!/bin/bash

THIS_DIR=`cd $(dirname $0) && pwd`
BUILDS_DIR=$THIS_DIR/../builds

if [ "$1" == "help" ]; then
  echo "USAGE: $0 [realhw|desktop|both(default)] [debug|release|all(default)]"
  exit 0
fi


die() { echo "$@" 1>&2 ; exit 1; }

function clean_builds {
  
  if [ -e $BUILDS_DIR ]; then
    echo "Deleting everything under $BUILDS_DIR ..."
    rm -rf $BUILDS_DIR
  fi
}

if [ "$1" == "clean" ]; then
  clean_builds
  shift
fi

if [ -z "$1" ]; then
   TARGET=both
else
   TARGET=$1
fi

if [ -z "$2" ]; then
    TYPE=debug
else
    TYPE=$2
fi

if [ ! -e $BUILDS_DIR ]; then
  mkdir -p $BUILDS_DIR
fi

./build_console.sh $TARGET $TYPE || die "Building gberry-console failed!"
./build_mobapp.sh both all || die "Building gberry-mobile failed!"
./build_game.sh $TARGET $TYPE testgame|| die "Building gberry-testgame failed!"
./build_game.sh $TARGET $TYPE react || die "Building gberry-react failed!"
./build_game.sh $TARGET $TYPE tictactoe|| die "Building gberry-tictactoe failed!"
./build_game.sh $TARGET $TYPE trivia|| die "Building gberry-trivia failed!"

echo "[DONE]"