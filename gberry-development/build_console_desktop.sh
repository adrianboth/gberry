#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`
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
  BUILD_NAME=Desktop_Qt_5_4_0_GCC_64bit-Debug
  CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"
  SPEC="-spec linux-g++"

  build gberry-lib gberry-lib/gberry-lib.pro
  build console-lib gberry-console/console-lib/console-lib.pro
  build comms gberry-console/comms/comms.pro
  build mainui gberry-console/mainui/mainui.pro
  
  $WORKSPACE_ROOT/gberry-console/qt5rpi/deploy_qt.sh desktop || die "Failed to deploy Qt!"
}

function build_rpi {
  QMAKE=/usr/local/qt5pi/bin/qmake
  if [ ! -e /mnt/rasp-pi-rootfs ]; then
    echo "ERROR: /mnt/rasp-pi-rootfs not mounted!"
    echo "USE:   sudo mount -o loop,offset=62914560 2014-12-24-wheezy-raspbian.img /mnt/rasp-pi-rootfs"
    exit 1
  fi
  BUILD_NAME=Raspberry-Debug
  CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"
  #SPEC=linux-g++
  SPEC=""
  
  build gberry-lib gberry-lib/gberry-lib.pro
  build console-lib gberry-console/console-lib/console-lib.pro
  build comms gberry-console/comms/comms.pro
  build mainui gberry-console/mainui/mainui.pro
  $WORKSPACE_ROOT/gberry-console/qt5rpi/deploy_qt.sh rpi || die "Failed to deploy Qt!"
}

# TODO: These are debug builds: release also

if [ "$1" == "desktop" ]; then
  build_desktop
elif [ "$1" == "rpi" ]; then
  build_rpi
else
  build_desktop
  build_rpi
fi
