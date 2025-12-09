#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('starbound', checksec=0)
#libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10202)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

p.interactive()

