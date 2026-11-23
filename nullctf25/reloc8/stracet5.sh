#!/bin/sh
timeout -k 5 30 strace -f -e trace=execve,exit,exit_group -s 200 ./d8 --allow-natives-syntax t5.js > strace.txt 2>&1
echo "EXIT=$?" >> strace.txt
grep -E "execve|exit" strace.txt | head -40
