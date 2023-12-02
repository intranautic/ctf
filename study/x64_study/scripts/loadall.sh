#!/bin/sh
for ko in $(ls | grep '\.ko'); do
  if [ -f $ko ]; then
    sudo dmesg -C
    sudo insmod $ko
    sudo rmmod $ko
    sudo dmesg
  fi
done

