#!/usr/bin/env python3
from pwn import *
from sys import argv
lib_path="/glibc/2.27/64/lib"
binary = ELF('tcache_tear', checksec=0)
libc = ELF('libc-2.27.so', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10207)
else:
  p = process([lib_path+'/ld-2.27.so', './tcache_tear'], \
    env={'LD_PRELOAD':'libc-2.27.so'})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

p.interactive()

