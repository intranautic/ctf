#!/usr/bin/env python3
import subprocess, sys, time

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./babyallocator232
set exec-wrapper env LD_LIBRARY_PATH=/home/folly/ctf/pwnable.tw/babyalloc
break *0x401088
commands
silent
printf "READINT n=%d buf=", $eax
output *(char(*)[16])(($rbp-0x20))
printf "\n"
continue
end
break *0x4010f5
commands
silent
printf "READRET n=%d\n", $eax
continue
end
break *0x4013a2
commands
silent
printf "SCANCALL rsp=%p\n", $rsp
continue
end
break *0x4013a7
commands
silent
printf "SCANRET rsp=%p\n", $rsp
continue
end
break *0x401350
commands
silent
printf "GUARDCHK rsp=%p size1=%p guard=%p\n", $rsp, $rax, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x40146a
commands
silent
printf "WRITE ptr=%p size=%d\n", $rdi, $esi
continue
end
break *0x401362
commands
silent
printf "MOREPATH req=%p rsp=%p\n", $rdi, $rsp
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

def until(tok, timeout=20):
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

try:
    until(b'Your choice:')
    send(b'1')
    until(b'Size :')
    send(b'512')
    until(b'allocator ? :')
    send(b'nn')
    until(b'Your choice:')
    send(b'3')
    until(b'Content :')
    send(b'C'*512)
    until(b'Your choice:')
    send(b'4')
    until(b'Your choice:')
    send(b'1')
    until(b'Size :')
    send(b'512')
    until(b'allocator ? :')
    send(b'nn2')
    until(b'Your choice:')
    send(b'3')
    until(b'Content :')
    send(b'B'*512)
    until(b'Your choice:')
    send(b'5')
    until(b'Your choice:')
    send(b'5')
    until(b'Your choice:')
    send(b'6')
except Exception as e:
    print('DRIVER ERR', e)
time.sleep(3)
try:
    while True:
        b = p.stdout.read(1)
        if not b: break
        sys.stdout.write(b.decode(errors='replace'))
except Exception:
    pass
p.wait()
