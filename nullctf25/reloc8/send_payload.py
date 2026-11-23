#!/usr/bin/env python3
# send exploit.b64 to 127.0.0.1:1337 and dump full response
import socket, sys, time

s = socket.create_connection(("127.0.0.1", 1337))
s.sendall(open("exploit.b64", "rb").read() + b"\n")
s.settimeout(12)
data = b""
try:
    while True:
        chunk = s.recv(4096)
        if not chunk:
            break
        data += chunk
except socket.timeout:
    pass
s.close()
out = data.decode(errors="replace")
# print full response but truncate the echoed b64 line if present
lines = out.split("\n")
for i, ln in enumerate(lines):
    if len(ln) > 200:
        ln = ln[:200] + "...[truncated " + str(len(ln)) + " chars]"
    print(repr(ln))
print("--- nullctf lines ---")
for ln in lines:
    if "nullctf" in ln:
        print(ln)
