#!/usr/bin/env python3
from pwn import *
from sys import argv
context.arch = 'amd64'
binary = ELF('babystack', checksec=0)
libc = ELF('libc_64.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10205)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

s(b'1', b'>>')




p.interactive()

