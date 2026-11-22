#!/bin/bash
cd /home/folly/ctf/pwnable.tw/hitcon_ftp
pkill -x ld-2.27.so; sleep 0.3
rm -f /tmp/gdbprobe.log
(setsid gdb -q -nx -batch -x /tmp/pn.gdb --args ./ld-2.27.so --library-path . ./hitcon_ftp 11113 >/tmp/gdbprobe.log 2>&1 </dev/null &)
sleep 2
python3 /tmp/send_probe.py
sleep 1.5
pkill -x gdb
sleep 0.3
echo "=== hits:"
grep -a -c 'HIT' /tmp/gdbprobe.log
