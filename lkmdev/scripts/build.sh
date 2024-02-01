#!/bin/sh
if [ $# -ne 1 ]; then
  printf "Usage: %s <rootfs>\n" $0
  exit
fi
if [ ! -d $1 ]; then
  echo "Please provide a valid rootfs path"
  exit
fi

OUT=../initramfs.cpio
cd $1
find . -print0 | cpio --null -ov --format=newc>$OUT && gzip -f $OUT
