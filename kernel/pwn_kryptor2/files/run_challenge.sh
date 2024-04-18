#!/bin/sh

qemu-system-x86_64 \
    -m 700M \
    -smp cores=4 \
    -cpu qemu64,+smep,+smap,+rdrand \
    -kernel /home/ctf/bzImage.debug \
    -append "console=ttyS0 root=/dev/sda quiet loglevel=3 oops=panic panic=-1 net.ifnames=0 pti=on" \
    -hda /home/ctf/secureos.qcow2 \
    -snapshot \
    -monitor /dev/null \
    -nographic \
    -no-reboot
