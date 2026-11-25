#!/usr/bin/env python3
import subprocess, sys, time

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./ld-2.23.so
set args --library-path . ./babyallocator
break *0x4013a2
commands
silent
printf "SCANCALL rsp=%p\n", $rsp
set $fill = $rsp - 0x2000
while $fill < $rsp
  set *(unsigned char*)$fill = 0xAA
  set $fill = $fill + 1
end
continue
end
break *0x4013a7
commands
silent
printf "SCANRET rsp=%p\n", $rsp
set $scan = $rsp - 0x2000
set $minmod = $rsp
while $scan < $rsp
  if *(unsigned char*)$scan != 0xAA
    if $scan < $minmod
      set $minmod = $scan
    end
  end
  set $scan = $scan + 1
end
printf "LOWEST MODIFIED depth = %d\n", $rsp - $minmod
set $scan = $rsp - 0x2000
while $scan < $rsp
  set $v = *(unsigned long*)$scan
  if ($v > 0x7ffff0000000) && ($v < 0x800000000000)
    printf "  [depth %d] = %p\n", $rsp - $scan, $v
  end
  set $scan = $scan + 8
end
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
