#!/bin/sh
# Run t5 with hard timeout, capture everything to t5out.txt inside WSL
timeout -k 5 30 ./d8 --allow-natives-syntax t5.js > t5out.txt 2>&1
echo "EXIT=$?" >> t5out.txt
cat t5out.txt
