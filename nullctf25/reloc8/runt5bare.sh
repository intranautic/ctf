#!/bin/sh
# Mimic server exactly: no flags, empty-ish env, stdout to socket->file
timeout -k 5 30 env -i ./d8 t5.js > t5bare.txt 2>&1
echo "EXIT=$?" >> t5bare.txt
cat t5bare.txt
