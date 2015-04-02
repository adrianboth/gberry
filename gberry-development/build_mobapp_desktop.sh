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
  $QMAKE $PRO_FILE -spec $SPEC $CONFIG || die "QMake failed!"
  /usr/bin/make || die "Make failed"
}

function build_desktop {
  # TODO: this should be configurable
  QMAKE=/home/tevuore/Qt5.4/5.4/gcc_64/bin/qmake
  SPEC=linux-g++
  BUILD_NAME=Desktop_Qt_5_4_0_GCC_64bit-Debug
  CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"
  
  build gberry-lib gberry-lib/gberry-lib.pro
  build mobapp gberry-mobile/mobapp/mobapp.pro
}

function build_android {
  QTDIR=/home/tevuore/Qt5.4/5.4/android_armv7
  QMAKE=$QTDIR/bin/qmake
  SPEC=android-g++
  BUILD_NAME=Android_for_armeabi_v7a_GCC_4_8_Qt_5_4_0-Debug
  CONFIG="CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug BUILD_NAME=$BUILD_NAME"

  export ANDROID_NDK_ROOT=/home/tevuore/android/android-ndk-r9d
  export ANDROID_SDK_ROOT=/home/tevuore/android/adt-bundle-linux-x86_64-20140702/sdk
  
  build gberry-lib gberry-lib/gberry-lib.pro
  build mobapp gberry-mobile/mobapp/mobapp.pro
  
  # TODO: apk packaging
  BUILD_DIR=${BUILDS_DIR}/build-mobapp-${BUILD_NAME}
  ANDROID_BUILD=$BUILD_DIR/android-build
  /usr/bin/make INSTALL_ROOT=$ANDROID_BUILD install
  
  # TODO: where this *.json comes?
  $QTDIR/bin/androiddeployqt --input $BUILD_DIR/android-libmobapp.so-deployment-settings.json --output $ANDROID_BUILD --deployment bundled --android-platform android-20 --jdk /usr/lib/jvm/java-7-openjdk-amd64 --ant /usr/bin/ant
}

if [ "$1" == "desktop" ]; then
  build_desktop
elif [ "$1" == "android" ]; then
  build_android
else
  build_desktop
  build_android
fi
