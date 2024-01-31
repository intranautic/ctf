#!/bin/sh
if [ $# -ne 2 ]
  then
    echo "Usage: ./build.sh <module name> <action>"
    echo "Actions: clean load unload run"
    exit 1
fi
make MODULE_NAME=$1 $2
