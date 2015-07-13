#!/bin/bash
THIS_DIR=`cd $(dirname $0) && pwd`
exec $THIS_DIR/waitapp $*
