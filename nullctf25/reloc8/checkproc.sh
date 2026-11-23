#!/bin/sh
pid=$(pgrep -n d8)
echo PID=$pid
if [ -n "$pid" ]; then
  head -5 /proc/$pid/status
  echo "cmdline: $(cat /proc/$pid/cmdline | tr '\0' ' ')"
  echo "wchan: $(cat /proc/$pid/wchan)"
  echo "exe: $(readlink /proc/$pid/exe)"
  ls -l /proc/$pid/fd 2>/dev/null | head -10
  echo "--- stack ---"
  cat /proc/$pid/stack 2>/dev/null | head -5
fi
