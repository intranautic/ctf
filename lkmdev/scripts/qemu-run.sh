#!/bin/sh
# run script for for x86_64, change as needed
# not a fucking bash script connoisseur cut me some slack ok
if [ $# -ne 2 ]; then
  printf "Usage: %s <linux build> <initrd>\n" $0
  exit
fi
if [ ! -f $2 ]; then
  echo "Please pass valid initrd path"
  exit -1
fi
BZIMAGE=$1/arch/x86/boot/bzImage
if [ ! -d $1 ] || [ ! -f $BZIMAGE ]; then
  echo "Invalid path or could not find bzImage in build path!"
  exit -1
fi
qemu-system-x86_64 \
  -kernel $BZIMAGE \
  -initrd $2 \
  -append "console=ttyS0 kaslr panic=-1" \
  -monitor none \
  -nographic \
  -no-reboot \
  -cpu qemu64,+smep,+smap \
  -m 256M \
  -s

