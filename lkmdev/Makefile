MODULE := smpcall
obj-m += $(MODULE).o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo dmesg -C
	sudo insmod $(MODULE).ko
	sudo dmesg -H

unload:
	sudo dmesg -C
	sudo rmmod $(MODULE).ko
	sudo dmesg -H

run: all load unload

.PHONY: clean load unload run
