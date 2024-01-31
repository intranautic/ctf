obj-m += $(MODULE_NAME).o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	./scripts/load.sh insmod $(MODULE_NAME)

unload:
	./scripts/load.sh rmmod $(MODULE_NAME)

run: all load unload

.PHONY: clean load unload run
