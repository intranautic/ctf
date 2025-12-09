#!/bin/sh
if [ $# -ne 1 ]; then
  printf "Usage: %s <rootfs>\n" $0
  exit
fi
if [ -d $1 ]; then
  (cd $1 && find . -print0 | cpio --null -ov --format=newc >../"$1.cpio")
  gzip -f "$1.cpio"
else
  echo "Please provide a valid rootfs path"
fi

