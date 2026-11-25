#!/usr/bin/env python3
import subprocess, sys, time, os

os.chdir('/home/folly/ctf/pwnable.tw/babyalloc')
open('sent.bin','wb').write(b'\xAA'*0x1400)

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./babyallocator232
set exec-wrapper env LD_LIBRARY_PATH=/home/folly/ctf/pwnable.tw/babyalloc
break *0x400e40
commands
silent
set $pc = *(unsigned long*)$rsp
set $rsp = $rsp + 8
set $rax = 0
continue
end
break *0x4013a2
commands
silent
restore sent.bin binary $rsp-0x1400
printf "SCANCALL rsp=%p\n", $rsp
continue
end
break *0x4013a7
commands
silent
printf "SCANRET rsp=%p\n", $rsp
dump binary memory scanband.bin $rsp-0x1400 $rsp
continue
end
run
quit
"""
open('gdbscript.txt','w').write(GDBS)

p = subprocess.Popen(['gdb','-q','-batch','-x','gdbscript.txt'],
                     stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                     stderr=subprocess.STDOUT, bufsize=0)

buf = b''
def send(chunk):
    p.stdin.write(chunk)
    p.stdin.flush()

def until(tok, timeout=30):
    global buf
    end = time.time() + timeout
    while time.time() < end:
        while tok in buf:
            i = buf.find(tok)
            out, buf2 = buf[:i+len(tok)], buf[i+len(tok):]
            buf = buf2
            sys.stdout.write(out.decode(errors='replace'))
            return True
        c = os.read(p.stdout.fileno(), 4096)
        if not c: break
        buf += c
    return False

for i in range(3):
    until(b'Your choice:'); send(b'1\n')
    until(b'Size :'); send(b'128\n')
    until(b'allocator ? :'); send(b'name%d\n' % i)
until(b'Your choice:'); send(b'6\n')
time.sleep(3)
try:
    while True:
        c = os.read(p.stdout.fileno(), 4096)
        if not c: break
        buf += c
except Exception:
    pass
sys.stdout.write(buf.decode(errors='replace')[-500:])
p.wait()

import struct
band = open('scanband.bin','rb').read()
assert len(band) == 0x1400, len(band)
low = None
for i in range(len(band)):
    if band[i] != 0xAA:
        low = i; break
print('LOWEST MODIFIED depth = 0x%x' % (0x1400 - low))
print('=== written slots (depth, value) — pointers ===')
for off in range(low & ~7, 0x1400, 8):
    v = struct.unpack('<Q', band[off:off+8])[0]
    if all(b == 0xAA for b in band[off:off+8]): continue
    depth = 0x1400 - off
    if 0x7f0000000000 < v < 0x800000000000:
        print('  depth 0x%04x = 0x%012x' % (depth, v))
