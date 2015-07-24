#!/bin/bash

THIS_DIR=`cd $(dirname $0) && pwd`
WORKSPACE_ROOT=`cd $THIS_DIR/.. && pwd`
BUILDS_DIR=$THIS_DIR/../builds

QTDIR=/home/tevuore/Qt5.4/5.4/android_armv7
BUILD_NAME=Android_for_armeabi_v7a_GCC_4_8_Qt_5_4_0-Release
COMP=mobapp
BUILD_DIR=$BUILDS_DIR/build-${COMP}-${BUILD_NAME}

if [ -z "$1" ]; then
  echo "ERROR: No device parameter provided"
  exit 1
fi

$QTDIR/bin/androiddeployqt --verbose --output $BUILD_DIR/android-build --no-build --input $BUILD_DIR/android-lib${COMP}.so-deployment-settings.json --reinstall --deployment bundled --device $1

# look devices by
#  $ /home/tevuore/android/adt-bundle-linux-x86_64-20140702/sdk/platform-tools/adb devices