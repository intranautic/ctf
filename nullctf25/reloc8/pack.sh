#!/bin/sh
cp t5.js exploit.js
base64 -w 0 exploit.js > exploit.b64
echo "b64 bytes: $(wc -c < exploit.b64)"
head -c 120 exploit.b64; echo "..."
