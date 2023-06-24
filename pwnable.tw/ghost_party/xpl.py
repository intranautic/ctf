#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('ghostparty', checksec=0)
libc = ELF('libc_64.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10401)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

def option_add(
    name: str=b'A'*8,
    age=0,
    msg=b'A'*8,
    kind=b'A'*8,
    attr=b'A'*8
  ):
  sl(b'1', b'Your choice :')
  sl(name, b':')


p.interactive()

