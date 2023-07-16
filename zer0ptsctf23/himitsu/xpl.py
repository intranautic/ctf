#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('chall', checksec=False)
#libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('misc2.2023.zer0pts.com',9003)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

def add_note(index: int):
  sl(b'1', b'>')
  sl(str(index).encode(), b'index: ')

def edit_note(index: int, data):
  sl(b'2', b'>')
  sl(str(index).encode(), b'index: ')
  sl(data.encode(), b'data: ')

free_idx=200


p.interactive()

