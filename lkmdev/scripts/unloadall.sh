#!/bin/sh
for ko in $(ls | grep '\.ko'); do
  if [ -f $ko ]; then
    sudo dmesg -C
    sudo rmmod $ko
    sudo dmesg -H
  fi
done

