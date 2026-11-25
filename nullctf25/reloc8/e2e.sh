#!/bin/sh
# End-to-end test: start listener, send payload, capture response
rm -f srv.log resp.txt
python3 mini_socat.py > srv.log 2>&1 &
SRVPID=$!
sleep 1
# send payload (one line) then keep connection open briefly to read response
(sed 's/$//' exploit.b64; sleep 8) | nc 127.0.0.1 1337 > resp.txt 2>&1 &
NCPID=$!
sleep 10
kill $NCPID 2>/dev/null
kill $SRVPID 2>/dev/null
pkill -f mini_socat.py 2>/dev/null
sleep 1
echo "--- resp.txt (first 400 chars per line, head) ---"
cut -c1-400 resp.txt | head -20
echo "--- flag lines ---"
grep -a "nullctf" resp.txt
