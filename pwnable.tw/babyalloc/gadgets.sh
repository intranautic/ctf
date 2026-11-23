#!/bin/bash
cd /home/folly/ctf/pwnable.tw/babyalloc
echo '=== pop rdi ==='; ROPgadget --binary babyallocator | grep -E ': pop rdi ; ret' | head -3
echo '=== pop rsi ==='; ROPgadget --binary babyallocator | grep -E ': pop rsi ; ret' | head -3
echo '=== pop rdx combos ==='; ROPgadget --binary babyallocator | grep -E 'pop rdx' | head -6
echo '=== pop rsp ==='; ROPgadget --binary babyallocator | grep -E ': pop rsp' | head -5
echo '=== ret ==='; ROPgadget --binary babyallocator | grep -E ': ret$' | head -3
echo '=== GOT entries ==='
python3 - <<'EOF'
from pwn import *
context.log_level='error'
e = ELF('./babyallocator', checksec=False)
# .got section 0x603ee8..0x604000 — map via relocations
for r in e.rel:
    pass
import subprocess
out = subprocess.run(['readelf','-rW','babyallocator'], capture_output=True, text=True).stdout
print(out[:2000])
EOF
