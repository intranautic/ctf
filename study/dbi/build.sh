#!/bin/sh
if [ $# -eq 0 ]
  then
    echo "Usage: ./build.sh <path_to_pin_root>"
    exit 1
fi
make PIN_ROOT=$1
