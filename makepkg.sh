#!/bin/bash

# first parameter is directory to package (typically release folder)
# second parameter is path to zip file

if [ -z $1 ]; then
    echo "ERROR: Please provide path to directory that will be packaged"
    exit 1
fi

if [ -z $2 ]; then
    echo "ERROR: Please provide zip file path"
    exit 1
fi

SRC_DIR=$1
DST_ZIP=$2

echo "Working directory $SRC_DIR ..."
cd $SRC_DIR
if [ $? != 0 ]; then
    echo "ERROR: Failed to change directory to $SRC_DIR"
    exit 1
fi

# TODO: we could have somehow crossplatform way to make packages

echo "Creating zip file $DST_ZIP ..."
zip -r $DST_ZIP .
if [ $? != 0 ]; then
    echo "ERROR: Failed to make zip in $SRC_DIR"
    exit 1
fi

echo "[DONE]"
