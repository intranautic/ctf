#!/bin/sh

qemu-system-x86_64 \
		-kernel ./bzImage.debug \
		-initrd ./dist.cpio.gz \
        -monitor /dev/null \
		-nographic -append "console=ttyS0, nokaslr" \
    -s
