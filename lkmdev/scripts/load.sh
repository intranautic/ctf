#!/bin/sh
sudo dmesg -C
sudo $1 "$2.ko"
sudo dmesg -H

