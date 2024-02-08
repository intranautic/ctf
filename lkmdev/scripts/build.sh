#!/bin/sh
if [ $# -ne 2 ]; then
  printf "Usage: %s <rootfs> <initramfs>\n" $0
  exit
fi
if [ ! -d $1 ]; then
  echo "Please provide a valid rootfs path"
  exit
fi

cd $1
find . -print0 | cpio --null -ov --format=newc>$2 && gzip -f $2
