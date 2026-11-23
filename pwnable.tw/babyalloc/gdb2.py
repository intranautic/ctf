#!/usr/bin/env python3
import subprocess, sys, time, re

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./babyallocator
break *0x401350
commands
silent
printf "GUARDCHK rsp=%p size1=%p guard=%p\n", $rsp, $rax, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x401369
commands
silent
printf "  STACKPATH rsp=%p\n", $rsp
continue
end
break *0x401367
commands
silent
printf "  MOREPATH block=%p rsp=%p guard=%p\n", $rax, $rsp, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x4017d0
commands
silent
printf "__generic_morestack framesize=%p param=%p rsp=%p guard=%p\n", *(unsigned long*)$rdi, $rdx, $rsp, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x401e30
commands
silent
printf "alloc_stack_space size=%p current=%p\n", $rdi, *(unsigned long*)($fs_base-16)
continue
end
break *0x40146a
commands
silent
printf "WRITE ptr=%p size=%d rsp=%p guard=%p\n", $rdi, $esi, $rsp, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x401482
commands
silent
printf "RECURSE rsp=%p guard=%p\n", $rsp, *(unsigned long*)($fs_base+0x70)
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
        if not b:
            break
        data += b
        sys.stdout.write(b.decode(errors='replace'))
        sys.stdout.flush()
        if tok in data:
            return data
    return data

# interaction
until(b'Your choice:')
send(b'1')
until(b'Size :')
send(b'512')
until(b'allocator ? :')
send(b'name1')
until(b'Your choice:')
send(b'3')
until(b'Content :')
send(b'C' * 512)
until(b'Your choice:')
send(b'4')
until(b'Your choice:')
send(b'1')
until(b'Size :')
send(b'512')
until(b'allocator ? :')
send(b'name2')
until(b'Your choice:')
send(b'3')
until(b'Content :')
send(b'D' * 512)
until(b'Your choice:')
send(b'5')
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
