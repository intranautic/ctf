#!/usr/bin/python3
from pwn import *

context.log_level = 'DEBUG'
def send_command(cmd, print_cmd = True, print_resp = False):
	if print_cmd:
		log.info(cmd)

	p.sendlineafter("$", cmd)
	resp = p.recvuntil("$")

	if print_resp:
		log.info(resp)

	p.unrecv("$")
	return resp

def send_file(src, dst):
	file = read(src)	
	f = b64e(file)

	send_command("rm -f {}.b64".format(dst))
	send_command("rm -f {}".format(dst))

	size = 800
	for i in range(len(f)//size + 1):
		log.info("Sending chunk {}/{}".format(i, len(f)//size))
		send_command("echo -n '{}' >> {}.b64".format(f[i*size:(i+1)*size], dst), False)

	send_command("cat {}.b64 | base64 -d > {}".format(dst, dst))

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("usage: ./send.py <IP> <PORT> <FILE TO SEND> <PATH ON REMOTE>")
        exit(-1)
    p = remote(sys.argv[1], int(sys.argv[2]))
    send_file(sys.argv[3], sys.argv[4])
    p.interactive()
