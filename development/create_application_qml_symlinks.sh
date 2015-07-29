#!/bin/bash
#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
ROOT_DIR=`cd $THIS_DIR/.. && pwd`

if [ -z $1 ]; then
    echo "ERROR: Please provide path to game folder"
    exit 1
fi

if [[ "$1" = /* ]]; then
	UI_DIR=$1/ui
else
    UI_DIR=$ROOT_DIR/$1/ui
fi

echo "Using path $UI_DIR"

if [ ! -e $UI_DIR ]; then
    mkdir $UI_DIR
fi

echo "Creating symlink $UI_DIR/console-lib"
ln -s $ROOT_DIR/gberry-console/console-lib/src/ui/qml $UI_DIR/console-lib
echo "Creating symlink $UI_DIR/gberry-lib"
ln -s $ROOT_DIR/gberry-lib/lib/src/ui/qml $UI_DIR/gberry-lib

echo "[DONE]"
