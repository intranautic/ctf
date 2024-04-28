all: build run


build:
	musl-gcc -static -o initramfs/xpl xpl.c
	./scripts/build.sh ./initramfs

run:
	./scripts/run.sh

.PHONY: build run
