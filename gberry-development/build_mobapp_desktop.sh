#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`

QMAKE=/home/tevuore/Qt5.4/5.4/gcc_64/bin/qmake
CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug"
SPEC=linux-g++
BUILDS_DIR=$THIS_DIR/../builds

# -- Build

die() { echo "$@" 1>&2 ; exit 1; }

function build {
  echo "------------------------------------------------------------------------------------------------------"
  echo "-- Component name: $1"
  COMP=$1
  echo "-- Pro file: $2"
  REL_PRO_FILE=$2
  
  SRC_DIR=$WORKSPACE_ROOT/$COMP
  BUILD_DIR=$BUILDS_DIR/build-${COMP}-Desktop_Qt_5_4_0_GCC_64bit-Debug
  PRO_FILE=$WORKSPACE_ROOT/$REL_PRO_FILE

  if [ ! -e $BUILD_DIR ]; then
    mkdir $BUILD_DIR
  fi

  cd $BUILD_DIR
  $QMAKE $PRO_FILE -spec $SPEC $CONFIG || die "QMake failed!"
  /usr/bin/make || die "Make failed"
}

build gberry-lib gberry-lib/gberry-lib.pro
build mobapp gberry-mobile/mobapp/mobapp.pro

# TODO: Qt
