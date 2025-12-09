#!/bin/sh
qemu-system-x86_64 \
  -drive if=pflash,file=ovmf.bin,format=raw \
  -drive file=fat:rw:efi,format=raw \
  -net none \
  -nographic \
  -monitor none \
  -s
