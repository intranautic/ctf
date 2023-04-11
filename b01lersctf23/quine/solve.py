#!/usr/bin/env python3
from pwn import *
from string import printable

flag = b'bctf{'

def test_flag(flag):
  p = ELF('a.out', checksec=0).process(argv=[flag])
  buffer = p.recvline().strip(b'\n').strip(b'X')[2:]
  p.close()
  return buffer

while flag[-1] != b'}':
  i = len(flag)
  for j in printable:
    if i < len(test_flag(flag + j.encode())):
      flag += j.encode()
      break
  print(flag)

