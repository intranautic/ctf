#!/bin/sh
rm -f srv.log child.log resp2.txt
python3 mini_socat.py > srv.log 2>&1 &
SRVPID=$!
sleep 1
python3 send_payload.py > resp2.txt 2>&1
kill $SRVPID 2>/dev/null
pkill -f mini_socat.py 2>/dev/null
echo "--- srv.log ---"; cat srv.log
echo "--- resp2.txt ---"; cat resp2.txt
