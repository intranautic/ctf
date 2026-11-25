#!/usr/bin/env python3
import subprocess, sys, time

GDBS = r"""
set pagination off
set confirm off
set debuginfod enabled off
file ./babyallocator
break *0x4013a2
commands
silent
printf "SCANCALL rsp=%p\n", $rsp
set $fill = $rsp - 0x1800
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
set $scan = $rsp - 0x1800
set $minmod = $rsp
while $scan < $rsp
  if *(unsigned char*)$scan != 0xAA
    if $scan < $minmod
      set $minmod = $scan
    end
  end
  set $scan = $scan + 1
end
printf "LOWEST MODIFIED = %p (depth %d)\n", $minmod, $rsp - $minmod
printf "STDINPTR writes:\n"
set $scan = $rsp - 0x1800
while $scan < $rsp
  set $v = *(unsigned long*)$scan
  if ($v > 0x7ffff7000000) && ($v < 0x800000000000)
    printf "  [%p] = %p\n", $scan, $v
  end
  set $scan = $scan + 8
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

# 3 alloca rounds to sample scanf depth each time
for i in range(3):
    until(b'Your choice:')
    send(b'1')
    until(b'Size :')
    send(b'128')
    until(b'allocator ? :')
    send(b'name%d' % i)
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
