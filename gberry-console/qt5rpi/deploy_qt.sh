#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`
BUILDS_DIR=$WORKSPACE_ROOT/../builds


if [ "$1" == "help" ]; then
  echo "USAGE: $0 [rpi|desktop|both(default)] [debug|release|all(default)]"
fi

# both desktop and raspberry version

function deploy_desktop {
  SRCDIR=/home/tevuore/Qt5.4/5.4/gcc_64
  
  BUILD_NAME=Desktop_Qt_5_4_0_GCC_64bit-$1
  DEPLOY_DIR=$BUILDS_DIR/deploy-console-$BUILD_NAME
  DSTDIR=$DEPLOY_DIR/lib/Qt/5.4.0
  
  if [ ! -e "$DSTDIR" ]; then
    mkdir -p $DSTDIR
  fi
  echo "Copying from $SRCDIR to $DSTDIR ..."  
  cp -r $SRCDIR/lib $DSTDIR/
  cp -r $SRCDIR/imports $DSTDIR/
  cp -r $SRCDIR/plugins $DSTDIR/
  cp -r $SRCDIR/qml $DSTDIR/
  cp -r $SRCDIR/translations $DSTDIR/
  cp -r $SRCDIR/libexec $DSTDIR/
}

function deploy_rpi {
  SRCDIR=$THIS_DIR/deploy/usr/local/qt5pi
  BUILD_NAME=Raspberry-$1
  DEPLOY_DIR=$BUILDS_DIR/deploy-console-${BUILD_NAME}
  DSTDIR=$DEPLOY_DIR/lib/Qt/5.4.0
  
  if [ ! -e "$DSTDIR" ]; then
    mkdir -p $DSTDIR
  fi
  echo "Copying from $SRCDIR to $DSTDIR ..."  
  cp -r $SRCDIR/* $DSTDIR/
}

function run_deploy {
  # $1 is run command, it is function
  # $2 is <empty>|debug|release|all
  
  if [ "$2" == "release" ] || [ "$2" == "Release" ]; then
    $1 Release
  elif [ "$2" == "all" ]; then
    $1 Debug
    $1 Release
  elif [ "$2" == "debug" ] || [ "$2" == "Debug" ] || [ "$2" == "" ]; then
    $1 Debug
  else
    echo "ERROR: Unknown deploy request: $2"
    exit 1
  fi
}


if [ "$1" == "desktop" ]; then
  run_deploy deploy_desktop $2
elif [ "$1" == "rpi" ]; then
  run_deploy deploy_rpi $2
else
  run_deploy deploy_desktop $2
  run_deploy deploy_rpi $2
fi