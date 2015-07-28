#!/bin/bash
#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
ROOT_DIR=`cd $THIS_DIR/../../.. && pwd`
cd $THIS_DIR/ui

ln -s $ROOT_DIR/gberry-console/console-lib/src/ui/qml console-lib
ln -s $ROOT_DIR/gberry-lib/lib/src/ui/qml gberry-lib

echo "[DONE]"
