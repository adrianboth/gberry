#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`
BUILDS_DIR=$THIS_DIR/../builds

if [ "$1" == "help" ]; then
  echo "USAGE: $0 [realhw|desktop|both(default)] [debug|release|all(default)]"
   exit 0
fi

# -- Build

die() { echo "$@" 1>&2 ; exit 1; }

function build {
  echo "------------------------------------------------------------------------------------------------------"
  echo "-- Component name: $1"
  COMP=$1
  echo "-- Pro file: $2"
  REL_PRO_FILE=$2
  
  SRC_DIR=$WORKSPACE_ROOT/$COMP
  BUILD_DIR=$BUILDS_DIR/build-${COMP}-${BUILD_NAME}
  PRO_FILE=$WORKSPACE_ROOT/$REL_PRO_FILE

  if [ ! -e $BUILD_DIR ]; then
    mkdir $BUILD_DIR
  fi

  cd $BUILD_DIR
  $QMAKE $PRO_FILE ${SPEC} ${CONFIG} BUILD_NAME=${BUILD_NAME} || die "QMake failed!"
  /usr/bin/make || die "Make failed"
  /usr/bin/make install || die "Make install failed"
}

function build_desktop {
  QMAKE=/home/tevuore/Qt5.4/5.4/gcc_64/bin/qmake
  BUILD_NAME=Desktop_Qt_5_4_0_GCC_64bit-$1
  
  if [ "$1" == "Debug" ]; then
    CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"
  else
    CONFIG="BUILD_NAME=$BUILD_NAME"
  fi
  SPEC="-spec linux-g++"

  if [ -z "$COMPONENT" ] || [ -z "$COMPONENT_PRO" ]; then
    build gberry-lib gberry-lib/gberry-lib.pro
    build console-lib gberry-console/console-lib/console-lib.pro
    build comms gberry-console/comms/comms.pro
    build mainui gberry-console/mainui/mainui.pro
    build waitapp gberry-console/waitapp/waitapp.pro
    
    $WORKSPACE_ROOT/gberry-console/qt5rpi/deploy_qt.sh desktop $1 || die "Failed to deploy Qt!"
    
  else 
    build $COMPONENT $COMPONENT_PRO
  fi
  
}

function build_rpi {
  QMAKE=/usr/local/qt5pi/bin/qmake
  if [ ! -e /mnt/rasp-pi-rootfs ]; then
    echo "ERROR: /mnt/rasp-pi-rootfs not mounted!"
    echo "USE:   sudo mount -o loop,offset=62914560 2014-12-24-wheezy-raspbian.img /mnt/rasp-pi-rootfs"
    exit 1
  fi
  BUILD_NAME=Raspberry-$1
  if [ "$1" == "Debug" ]; then
    CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"
  else
    CONFIG="BUILD_NAME=$BUILD_NAME"
  fi
  #SPEC=linux-g++
  SPEC=""
  
  build gberry-lib gberry-lib/gberry-lib.pro
  build console-lib gberry-console/console-lib/console-lib.pro
  build comms gberry-console/comms/comms.pro
  build mainui gberry-console/mainui/mainui.pro
  build waitapp gberry-console/waitapp/waitapp.pro
  
  # deployment happens for both debug and release if requested
  $WORKSPACE_ROOT/gberry-console/qt5rpi/deploy_qt.sh rpi $1 || die "Failed to deploy Qt!"
}

function run_build {
  # $1 is run command, it is function
  # $2 is <empty>|debug|release|all
  
  if [ "$2" == "release" ]; then
    $1 Release
  elif [ "$2" == "all" ]; then
    $1 Debug
    $1 Release
  elif [ "$2" == "debug" ] || [ "$2" == "" ]; then
    $1 Debug
  else
    echo "ERROR: Unknown build request: $2"
    exit 1
  fi
}

if [ "$1" == "desktop" ]; then
  run_build build_desktop $2
elif [ "$1" == "realhw" ]; then
  run_build build_rpi $2
else
  run_build build_desktop $2
  run_build build_rpi $2
fi
