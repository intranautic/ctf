#!/usr/bin/env python3
import subprocess, sys, time

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./babyallocator
break *0x401350
commands
silent
printf "GUARDCHK rsp=%p size1=%p guard=%p cur=%p\n", $rsp, $rax, *(unsigned long*)($fs_base+0x70), *(unsigned long*)($fs_base-16)
continue
end
break *0x401367
commands
silent
printf "  MOREPATH block=%p guard=%p\n", $rax, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x4017d0
commands
silent
printf "GENERIC_MORESTACK framesize=%p param=%p rsp=%p guard=%p cur=%p\n", *(unsigned long*)$rdi, $rdx, $rsp, *(unsigned long*)($fs_base+0x70), *(unsigned long*)($fs_base-16)
continue
end
break *0x401641
commands
silent
printf "  generic_morestack->ret=%p\n", $rax
continue
end
break *0x401e30
commands
silent
printf "ALLOC_STACK_SPACE size=%p cur=%p segs=%p\n", $rdi, *(unsigned long*)($fs_base-16), *(unsigned long*)($fs_base-8)
continue
end
break *0x40146a
commands
silent
printf "WRITE ptr=%p size=%d rsp=%p guard=%p\n", $rdi, $esi, $rsp, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x4014a4
commands
silent
printf "FREE ptr=%p rsp=%p\n", $rdi, $rsp
continue
end
run
quit
"""
open('gdbscript.txt','w').write(GDBS)

p = subprocess.Popen(['gdb','-q','-batch','-x','gdbscript.txt'],
                     stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                     stderr=subprocess.STDOUT, bufsize=0)

def send(line):
    p.stdin.write(line + b'\n')
    p.stdin.flush()

def until(tok, timeout=15):
    data = b''
    end = time.time() + timeout
    while time.time() < end:
        b = p.stdout.read(1)
        if not b: break
        data += b
        sys.stdout.write(b.decode(errors='replace'))
        sys.stdout.flush()
        if tok in data: return data
    return data

DEPTH = 8
for i in range(DEPTH):
    until(b'Your choice:')
    send(b'1')
    until(b'Size :')
    send(b'4095')            # 0xfff alloca
    until(b'allocator ? :')
    send(b'name%d' % i)
    until(b'Your choice:')
    send(b'3')
    until(b'Content :')
    send(b'X' * 4095)
    until(b'Your choice:')
    send(b'4')

for i in range(DEPTH):
    until(b'Your choice:')
    send(b'5')
until(b'Your choice:')
send(b'6')
time.sleep(2)
try:
    while True:
        b = p.stdout.read(1)
        if not b: break
        sys.stdout.write(b.decode(errors='replace'))
except Exception:
    pass
p.wait()
