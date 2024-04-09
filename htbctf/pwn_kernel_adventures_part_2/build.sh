#!/bin/sh
if [ $# -ne 1 ]; then
  printf "Usage: %s <rootfs>\n" $0
  exit
fi
if [ ! -d $1 ]; then
  echo "Please provide a valid rootfs path"
  exit
fi

(cd $1 && find . -print0 | cpio --null -ov --format=newc >../dist.cpio)
gzip -f dist.cpio
