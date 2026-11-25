#!/usr/bin/env python3
from pwn import *

context.arch = 'amd64'
context.log_level = 'info'

p = process('./babyallocator')

def menu(c):
    p.recvuntil(b'Your choice:')
    p.sendline(str(c).encode())

def alloca_stack(size, name=b'A'):
    menu(1)
    p.recvuntil(b'Size :')
    p.sendline(str(size).encode())
    p.recvuntil(b'allocator ? :')
    p.sendline(name)

def alloca_heap(size, name=b'B'):
    menu(2)
    p.recvuntil(b'Size :')
    p.sendline(str(size).encode())
    p.recvuntil(b'allocator ? :')
    p.sendline(name)

def write(data):
    menu(3)
    p.recvuntil(b'Content :')
    p.send(data)

def new_alloc():
    menu(4)

def release():
    menu(5)

if len(sys.argv) > 1 and sys.argv[1] == 'gdb':
    # interactive mode for gdb attach style testing
    pass

# simple session: alloca on stack of 0x200, write, release
alloca_stack(0x200, b'name1')
write(b'C' * 0x200 + b'\n')
new_alloc()          # child allocator
alloca_stack(0x200, b'name2')
write(b'D' * 0x200 + b'\n')
release()             # child releases
release()             # parent releases
menu(6)
p.interactive()
