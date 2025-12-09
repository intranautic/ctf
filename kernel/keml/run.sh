#!/bin/sh

BUILDS=/home/vagrant/mini_linux/
LINUX_BUILD=$BUILDS/linux/

musl-gcc -static -o rootfs/xpl xpl.c
build_initramfs.sh ./rootfs
qemu-system-x86_64 \
	-cpu qemu64,-smep,-smap \
	-kernel bzImage \
	-initrd initramfs.cpio.gz \
	-nographic \
	-append "root=/dev/ram rw console=ttyS0 oops=panic loglevel=2 panic=1 console=ttyS0" \
	-monitor /dev/null
