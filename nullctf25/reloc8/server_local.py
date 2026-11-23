#!/usr/bin/env python3

import base64
import os
import tempfile

if __name__ == "__main__":
    exploit = input("Code (base64): ")
    print(exploit)
    exploit = base64.b64decode(exploit.encode())
    with tempfile.NamedTemporaryFile(delete=False) as file:
        file.write(exploit)
        file.close()
    os.execve("./d8", ["./d8", file.name], {})

