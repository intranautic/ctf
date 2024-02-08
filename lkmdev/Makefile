MODULE_NAME := smpcall
MODULE_LINUX=/lib/modules/$(shell uname -r)/build
ROOTFS=./rootfs
INITRAMFS=initramfs.cpio
LINUX_BUILD=$(MODULE_LINUX)
PWD := $(CURDIR)

obj-m += $(MODULE_NAME).o

all:
	make -C $(MODULE_LINUX) M=$(PWD) modules

clean:
	make -C $(MODULE_LINUX) M=$(PWD) clean

load:
	sudo dmesg -C
	sudo insmod $(MODULE_NAME).ko
	sudo dmesg -H

unload:
	sudo dmesg -C
	sudo rmmod $(MODULE_NAME).ko
	sudo dmesg -H

buildfs: all
	cp -f $(MODULE_NAME).ko $(ROOTFS)
	./scripts/build.sh $(ROOTFS) ../$(INITRAMFS)

qemurun: buildfs
	./scripts/qemu-run.sh $(LINUX_BUILD) ./$(INITRAMFS).gz

run: all load unload 

.PHONY: clean load unload buildfs qemurun
