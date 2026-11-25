#!/bin/sh
timeout -k 5 90 gdb --batch \
  -ex "set confirm off" \
  -ex "set pagination off" \
  -ex "catch syscall execve" \
  -ex "run" \
  -ex "printf \"=== CAUGHT EXECVE ===\n\"" \
  -ex "info registers rip rsp rax rdi rsi rdx r13 r14 r15" \
  -ex "printf \"path str: \"" \
  -ex "x/s \$rdi" \
  -ex "printf \"argv[0] ptr: \"" \
  -ex "x/gx \$rsi" \
  -ex "printf \"argv[0] str: \"" \
  -ex "x/s *(char**)\$rsi" \
  -ex "printf \"argv[1] str: \"" \
  -ex "x/s *(char**)(\$rsi+8)" \
  -ex "printf \"argv[2] str: \"" \
  -ex "x/s *(char**)(\$rsi+16)" \
  -ex "printf \"stack top: \"" \
  -ex "x/6gx \$rsp" \
  -ex "printf \"shellcode at r13-32: \"" \
  -ex "x/4gx \$r13-32" \
  --args ./d8 --allow-natives-syntax t5.js > gdb2.txt 2>&1
echo "GDBEXIT=$?" >> gdb2.txt
tail -60 gdb2.txt
