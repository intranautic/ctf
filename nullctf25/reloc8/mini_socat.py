#!/usr/bin/env python3
# Mini-socat: TCP-LISTEN:1337,fork EXEC:server_local.py
import os, socket, sys

srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
srv.bind(("127.0.0.1", 1337))
srv.listen(4)
print("listening on 127.0.0.1:1337", flush=True)

while True:
    conn, _ = srv.accept()
    pid = os.fork()
    if pid == 0:
        srv.close()
        os.dup2(conn.fileno(), 0)
        os.dup2(conn.fileno(), 1)
        os.dup2(conn.fileno(), 2)
        conn.close()
        os.execve("/usr/bin/env", ["/usr/bin/env", "python3", "server_local.py"], {})
        os._exit(1)
    conn.close()
