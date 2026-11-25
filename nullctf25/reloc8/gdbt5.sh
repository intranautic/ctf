#!/bin/sh
rm -f gdbcmds.txt
gdb --batch \
  -ex "set confirm off" \
  -ex "set pagination off" \
  -ex "run" \
  -ex "echo ===POST-MORTEM===\n" \
  -ex "info registers rip rsp rax rbx rcx rdx rsi rdi r13 r14 r15" \
  -ex "x/30i \$rip-32" \
  -ex "info signal SIGSEGV" \
  --args ./d8 --allow-natives-syntax t5.js 2>&1 | tail -80
