#!/bin/sh
pid=$(pgrep -n d8)
echo "PID=$pid"
echo "--- rwx regions ---"
grep 'rwx' /proc/$pid/maps
echo "--- all exec anon regions ---"
awk '$2 ~ /x/ && NF == 5 {print}' /proc/$pid/maps | head -40
