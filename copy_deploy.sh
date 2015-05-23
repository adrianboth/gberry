#!/bin/bash

# $ bcopy_deploy.sh

THIS_DIR=`cd $(dirname $0) && pwd`

BUILDS_DIR=$THIS_DIR/../builds
DEPLOY_PROJECT=$THIS_DIR/../gberry
die() { echo "$@" 1>&2 ; exit 1; }

echo "Copying mobile application ..."
cp -r $BUILDS_DIR/build-mobapp-Android_for_armeabi_v7a_GCC_4_8_Qt_5_4_0-Release/android-build/bin/QtApp-debug.apk $DEPLOY_PROJECT/mobapp/GBerryMobApp.apk || die "Copying mobapp failed!"

echo "Copying console software ..."
cp -r $BUILDS_DIR/deploy-console-Raspberry-Release/* $DEPLOY_PROJECT/console/ || die "Copying console software failed!"

TARBALL=$BUILDS_DIR/gberry.tar.gz
if [ -e $TARBALL ]; then
  rm $TARBALL
fi

cd $DEPLOY_PROJECT
tar cvfz $TARBALL *

echo "[DONE]"