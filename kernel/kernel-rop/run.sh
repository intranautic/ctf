#!/bin/sh
gcc -static -o initramfs/xpl xpl.c
cd initramfs;
find . | cpio -o --format=newc > ../initramfs.cpio;
cd ..;
gzip -vf1 initramfs.cpio;

qemu-system-x86_64 \
    -m 128M \
    -cpu kvm64,+smep,+smap \
    -kernel vmlinuz \
    -initrd initramfs.cpio.gz \
    -snapshot \
    -nographic \
    -monitor /dev/null \
    -no-reboot \
    -append "console=ttyS0 kaslr kpti=1 quiet panic=1" \
    -s;
