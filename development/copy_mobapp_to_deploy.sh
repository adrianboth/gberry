#!/bin/bash

THIS_DIR=`cd $(dirname $0) && pwd`
BUILDS_DIR=$THIS_DIR/../builds

ANDROID_BUILD_NAME=Android_for_armeabi_v7a_GCC_4_8_Qt_5_4_0-Release
COMP=mobapp
BUILD_DIR=$BUILDS_DIR/build-${COMP}-${ANDROID_BUILD_NAME}
DST_FILENAME=gberry.apk

cp $BUILD_DIR/android-build/bin/QtApp-debug.apk $BUILDS_DIR/deploy-console-Raspberry-Release/www/${DST_FILENAME}
cp $BUILD_DIR/android-build/bin/QtApp-debug.apk $BUILDS_DIR/deploy-console-Desktop_Qt_5_4_0_GCC_64bit-Debug/www/${DST_FILENAME}


echo "[DONE]"
