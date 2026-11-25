#!/bin/bash
cd /home/folly/ctf/pwnable.tw/hitcon_ftp
pkill -x ld-2.27.so; sleep 0.3
setsid ./ld-2.27.so --library-path . ./hitcon_ftp 11115 >/dev/null 2>&1 </dev/null &
sleep 0.8
P=$(pgrep -f "hitcon_ftp 11115" | head -1)
echo "PID=$P"
cat /proc/$P/maps
kill $P 2>/dev/null
