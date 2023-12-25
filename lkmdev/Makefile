obj-m += segments.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	./scripts/loadall.sh

unload:
	./scripts/unloadall.sh

run: all load unload

.PHONY: clean load unload run
