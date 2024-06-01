#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('kidvm', checksec=False)
#libc = ELF()
s = lambda x, r="": \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="": \
  p.sendlineafter(r, x) if r else p.sendline(x)
log_addr = lambda name, addr: \
  log.info("%s: %#lx"%(name, addr))
if len(argv) >= 2 and argv[1] == '-r':
  p = remote()
else:
  p = binary.process(env={})



p.interactive()

