s4r's encrypted_box
https://crackmes.one/crackme/64f1f7dbd931496abf90952d

# Challenge

x64 Linux executable

Blank prompt.  Takes in some characters and dies.

# Diving in

Opening in IDA shows us it reads in 16 characters to a buffer pointed to by rsp:
```
.text:0000000000400078                 xor     rdi, rdi
.text:000000000040007B                 mov     rsi, rsp
.text:000000000040007E                 mov     rdx, 10h
.text:0000000000400085                 xor     rax, rax
.text:0000000000400088                 syscall                 ; LINUX - sys_read
```

After that we get a lot of this sort of thing:
```
.text:000000000040008A                 ror     r10, 8
.text:000000000040008E                 mov     r10b, 1
.text:0000000000400091                 rdtsc
.text:0000000000400093                 ror     r8, 8
.text:0000000000400097                 mov     r8b, 5Ch ; '\'
```

It's building some things by moving in a byte at a time and rotating them into place.  
None of this seems to involve our buffer, so there's no point digging into it.  
We can just get the final result from the debugger if we need it.

Our buffer shows up again here, with loc_40020E going to an exit syscall,
so whatever is happening here better evaluate to 0:
```
.text:00000000004001B8                 pinsrq  xmm2, qword ptr [rsp+8], 1
.text:00000000004001C1                 pinsrq  xmm2, qword ptr [rsp], 0
.text:00000000004001C9                 aesdec  xmm2, xmm1
.text:00000000004001CE                 comiss  xmm2, xmm0
.text:00000000004001D1                 jnz     short loc_40020E
```

The pinsrq instruction treats xmm2 as a qword array and inserts the second operand into the position specified by the third.
https://shell-storm.org/x86doc/PINSRB_PINSRD_PINSRQ.html

What's happening is xmm2 = (qword(password+8) << 64) | qword(password)
The entire 16-byte password is being read into xmm2 as though it were one big 128-bit number.

The aesdec instruction does one round of AES decryption on our password in xmm2 using a key in xmm2
(technically the using equivalent inverse key needed to decrypt a value that was encrypted with xmm2.)
https://shell-storm.org/x86doc/AESDEC.html

The comiss instruction compares the lower qwords of xmm2 and xmm0 as single-precision floating-point values.
https://shell-storm.org/x86doc/COMISS.html

The comparison is only checking the lower half of the register values, but we may get lucky
and have xmm0 contain the full result of aesdec we need to solve the password.

If we avoid the death jump, we setup a loop:
```
.text:00000000004001D1                 jnz     short loc_40020E
.text:00000000004001D3                 mov     rcx, 66B80h
.text:00000000004001DA                 pinsrq  xmm2, qword ptr [rsp+8], 1
.text:00000000004001E3                 pinsrq  xmm2, qword ptr [rsp], 0
```

The password is loaded into xmm2 the same way as before and rcx is initialized as a counter. 
Then we have the loop body:
```
.text:00000000004001EB loc_4001EB:                             ; CODE XREF: .text:000000000040020A↓j
.text:00000000004001EB                 sub     rcx, 10h
.text:00000000004001EF                 lea     rax, loc_400217
.text:00000000004001F6                 add     rax, rcx
.text:00000000004001F9                 movdqu  xmm0, xmmword ptr [rax]
.text:00000000004001FD                 aesdec  xmm0, xmm2
.text:0000000000400202                 movdqu  xmmword ptr [rax], xmm0
.text:0000000000400206                 cmp     rcx, 0
.text:000000000040020A                 jnz     short loc_4001EB
.text:000000000040020C                 jmp     short loc_400217
```

This loop decrypts the program text from loc_400217 through loc_400217 + 0x66b80 (the rest of the .text segment)
in 16-byte (128-bit) blocks using xmm2 (from our password buffer) as the decryption key, 
then jumps to it.

# First password recovery

Let's look back at the initial check we have to pass:
```
.text:00000000004001B8                 pinsrq  xmm2, qword ptr [rsp+8], 1
.text:00000000004001C1                 pinsrq  xmm2, qword ptr [rsp], 0
.text:00000000004001C9                 aesdec  xmm2, xmm1
.text:00000000004001CE                 comiss  xmm2, xmm0
.text:00000000004001D1                 jnz     short loc_40020E
```

If we open the target in gdb, break on 4001CE, and check the registers, we can see that:
```
key (xmm1) = 0xa16f50c4d1bfe9d0a8d01d9776210139
required decrypted value (xmm0) = 0x6d65ed24c22914c98e33d4cb8ec85c19
```

If we go ahead and assume xmm0 holds the full decrypted value despite only part of it being compared,
we could recover the initial password if we could calculate:
```
password (as uint128) = INVERSE_aesdec( data {0x6d65ed24c22914c98e33d4cb8ec85c19}, key {0xa16f50c4d1bfe9d0a8d01d9776210139})
```

Unfortunately aesdec doesn't have a clean inverse instruction.

The instruction performs one intermediate round of AES decryption via the Equivalent Inverse Cipher, 
which uses a modified key schedule to the one used for encryption and correctly performs decryption across 
all rounds, but no individual intermediate decryption round directly inverses any of the encryption rounds and visa versa.

It's difficult to fully cover the encryption and decryption schemes here, but for a good starting point see 
https://blog.nindalf.com/posts/implementing-aes/

We are going to focus on what specifically the Intel instruction does and figure out how to invert it.
Most of this information comes verbatim from 
https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf

From the paper, aesdec does the following:
```
AESDEC xmm1, xmm2/m128
Tmp := xmm1
Round Key := xmm2/m128
Tmp := InvShift Rows (Tmp)
Tmp := InvSubBytes (Tmp)
Tmp := InvMixColumns (Tmp)
xmm1 := Tmp xor Round Key
```

The order of InvShiftRows and InvSubBytes is interchangable, since the first just moves bytes around, 
and the second just replaces each byte with another from a lookup table.

There is another instruction we can use to partly unravel this, aesenclast:
```
AESENCLAST xmm1, xmm2/m128
Tmp := xmm1
Round Key := xmm2/m128
Tmp := Shift Rows (Tmp)
Tmp := SubBytes (Tmp)
xmm1 := Tmp xor Round Key
```

We can use this to invert InvShiftRows and InvSubBytes, but we still need a way to invert InvMixColumns 
in order to recover the original data.

If we are faced with:
```
aesdec xmm2, xmm1
; if (xmm2 out != xmm0) exit();
```

Then so far we can string together the above instructions like this:
```
;xmm0 == required out data

xor xmm0, xmm1			; Undo the XOR with key (xmm1) at the end of aesdec

;MixColumns				; Still need to find a way to accomplish this

xor rax, rax			; Perform aesenclast with key = 0 to bypass its final XOR with key
aesenclast xmm0, rax	; This will undo InvShiftRows and InvSubBytes

; xmm0 == password (as uint128)
```

Now we just need an isolated way to perform MixColumns to tie these together.

There are two other instructions we can use to our advantage here:
```
AESDECLAST xmm1, xmm2/m128
State := xmm1
Round Key := xmm2/m128
Tmp := InvShift Rows (State)
Tmp := InvSubBytes (Tmp)
xmm1:= Tmp xor Round Key 

AESENC xmm1, xmm2/m128
Tmp := xmm1
Round Key := xmm2/m128
Tmp := ShiftRows (Tmp)
Tmp := SubBytes (Tmp)
Tmp := MixColumns (Tmp)
xmm2 := Tmp xor Round Key
```

If we use aesdeclast to pre-invert ShiftRows and SubBytes on the supplied data, 
then feed that into aesenc, both with key = 0, then we will have isolated the MixColumns operation by itself!

So the full inverse of 'aesdec xmm2, xmm1 == xmm0' becomes:
```
;xmm0 = required out data
;xmm1 = key

xorps xmm0, xmm1		; Undo the XOR with key (xmm1) at the end of aesdec

xor rax, rax			; Set the key to 0
pinsrq xmm1, rax, 1
pinsrq xmm1, rax, 0

aesdeclast xmm2, xmm1	; Undo InvMixColumns
aesenc xmm2, xmm1
aesenclast xmm2, xmm1	; Undo InvShiftRows and InvSubBytes

; xmm2 == password (as uint128)
```
We could of course manually write out routines to do all this in C/C++, 
but this way takes advantage of native processor functionality and is simpler to explain here.

The full program utilizing intrinsics is included at tools/manual/inv_aesdec.cpp
```
$./inv_aesdec a16f50c4d1bfe9d0a8d01d9776210139 6d65ed24c22914c98e33d4cb8ec85c19
0x33932c78135ced62756fd627768ebd4e
4e bd 8e 76 27 d6 6f 75 62 ed 5c 13 78 2c 93 33
```

Using this string as our password passes the first test and appears to 
decrypt the next section of the code into something sensible.

# Second password recovery

We need to speed run past the first decryption stage:
```
$ gdb ./encrypted_box
(gdb) starti
...
(gdb) b *0x4001b8
Breakpoint 1 at 0x4001b8
(gdb) c
Continuing.
ABCDEFGHIJKLMNO

Breakpoint 1, 0x00000000004001b8 in ?? ()
(gdb) set {long}$rsp = 0x756fd627768ebd4e
(gdb) set {long}($rsp + 8)  = 0x33932c78135ced62
(gdb) x /16b $rsp
0x7fffffffe130:	0x4e	0xbd	0x8e	0x76	0x27	0xd6	0x6f	0x75
0x7fffffffe138:	0x62	0xed	0x5c	0x13	0x78	0x2c	0x93	0x33
(gdb) hbreak *0x400217

Hardware assisted breakpoint 2 at 0x400217
(gdb) c

Continuing.

Breakpoint 3, 0x0000000000400217 in ?? ()

(gdb) x /10i $pc

=> 0x400217:	xor    rdi,rdi
   0x40021a:	mov    rsi,rsp
   0x40021d:	mov    rdx,0x10
   0x400224:	xor    rax,rax
   0x400227:	syscall 
   0x400229:	ror    r9,0x0
   0x40022d:	mov    r9b,0x82
   0x400230:	rdtsc  
   0x400232:	ror    r11,0x38
   0x400236:	mov    r11b,0x42
(gdb) 
```

Nice.  The second section decrypts properly and looks like it's setup the same way as the first section.

If we open the disassembly listing with "layout asm" and scroll down, we can see we're going to need to solve for another key:
```
   0x40034b:	pinsrq xmm2,QWORD PTR [rsp+0x8],0x1
   0x400354:	pinsrq xmm2,QWORD PTR [rsp],0x0
   0x40035c:	aesenc xmm2,xmm1
   0x400361:	comiss xmm2,xmm0
   0x400364:	jne    0x4003a1
   0x400366:	mov    rcx,0x669e0
```

This time we have to find the inverse of aesenc, which is actually more straightforward than before 
since Intel has a standalone aesimc instruction to compute InvMixColumns (it's normally used for computing Inverse Cipher key schedules.)

Our target is the value of xmm0 when we're about to execute 400361 
(we'll backup a bit to 40034b so we can write in the input once we solve it.)
We also need the key from xmm1.

Continuing from where we left off above:
(ignore the discrepancies in breakpoint numbering, as these are compiled from several runs)
```
(gdb) hbreak *0x40034b

Hardware assisted breakpoint 4 at 0x40034b
(gdb) c

Continuing.
ABCDEFGHIJKLMNO

Breakpoint 4, 0x000000000040034b in ?? ()

(gdb) info registers xmm0

xmm0  ...   uint128 = 0xdcc5395bbc56682428716fed8251b59
(gdb) info registers xmm1

xmm1  ...   uint128 = 0x42244286f11c4d34e119759fa572a0ba
```

Let's work through finding the inverse:
```
AESENC xmm1, xmm2/m128
Tmp := xmm1
Round Key := xmm2/m128
Tmp := ShiftRows (Tmp)
Tmp := SubBytes (Tmp)
Tmp := MixColumns (Tmp)
xmm2 := Tmp xor Round Key
```

So we need to undo the final XOR, run that through InvMixColumns, then run InvShiftRows and InvSubBytes on that.

The XOR and InvMixColumns are simple enough:
```
xorps xmm0, xmm1     ; Undo final XOR in aesenc
aesimc xmm0, xmm0    ; Undo MixColumns
```

Now we just need InvShiftRows and InvSubBytes.
We already found a way to isolate those when solving the first password:
```
AESDECLAST xmm1, xmm2/m128
State := xmm1
Round Key := xmm2/m128
Tmp := InvShift Rows (State)
Tmp := InvSubBytes (Tmp)
xmm1:= Tmp xor Round Key 
```

So our entire inverse aesenc looks like:
```
;xmm0 = required out data
;xmm1 = key

xorps xmm0, xmm1        ; Undo final XOR in aesenc
aesimc xmm0, xmm0       ; Undo MixColumns

xor rax, rax            ; Set the key to 0
pinsrq xmm1, rax, 1
pinsrq xmm1, rax, 0

aesdeclast xmm0, xmm1   ; Undo ShiftRows and SubBytes
; xmm2 == password (as uint128)
```

This is implemented similarly to our last program, which you'll find at  tools/manual/inv_aesenc.cpp:
```
$ ./inv_aesenc 42244286f11c4d34e119759fa572a0ba dcc5395bbc56682428716fed8251b59
0x47ebd94245958fb6d4dc11f7808bd8bd
bd d8 8b 80 f7 11 dc d4 b6 8f 95 45 42 d9 eb 47
```

We again patch this into memory to verify it's correct:
```
(gdb) x /5i $pc

=> 0x40034b:   pinsrq xmm2,QWORD PTR [rsp+0x8],0x1
   0x400354:   pinsrq xmm2,QWORD PTR [rsp],0x0
   0x40035c:   aesenc xmm2,xmm1
   0x400361:   comiss xmm2,xmm0
   0x400364:   jne    0x4003a1

(gdb) set {long}$rsp = 0xd4dc11f7808bd8bd
(gdb) set {long}($rsp + 8) = 0x47ebd94245958fb6
(gdb) x /16b $rsp

0x7fffffffe130:   0xbd  0xd8  0x8b  0x80  0xf7  0x11  0xdc  0xd4
0x7fffffffe138:   0xb6  0x8f  0x95  0x45  0x42  0xd9  0xeb  0x47
(gdb) si  /* repeated until we get past the conditional jne*/

(gdb) x /12i $pc

=> 0x400366:   mov    rcx,0x669e0
   0x40036d:   pinsrq xmm2,QWORD PTR [rsp+0x8],0x1
   0x400376:   pinsrq xmm2,QWORD PTR [rsp],0x0
   0x40037e:   sub    rcx,0x10
   0x400382:   lea    rax,[rip+0x21]        # 0x4003aa
   0x400389:   add    rax,rcx
   0x40038c:   movdqu xmm0,XMMWORD PTR [rax]
   0x400390:   aesenc xmm0,xmm2
   0x400395:   movdqu XMMWORD PTR [rax],xmm0
   0x400399:   cmp    rcx,0x0
   0x40039d:   jne    0x40037e
   0x40039f:   jmp    0x4003aa
(gdb) hbreak *0x4003aa

Hardware assisted breakpoint 5 at 0x4003aa
(gdb)(gdb) c

Continuing.

Breakpoint 5, 0x00000000004003aa in ?? ()

(gdb) x /10i $pc

=> 0x4003aa:   xor    rdi,rdi
   0x4003ad:   mov    rsi,rsp
   0x4003b0:   mov    rdx,0x10
   0x4003b7:   xor    rax,rax
   0x4003ba:   syscall 
   0x4003bc:   ror    r10,0x10
   0x4003c0:   mov    r10b,0x96
   0x4003c3:   rdtsc  
   0x4003c5:   ror    r11,0x10
   0x4003c9:   mov    r11b,0x9
(gdb) 
```

Cool.  Let's see what we have to deal with this time.

# Third password recovery

We can hop back over to the assembly view and scroll down to find:
```
(gdb) x /5i 0x4004e6

   0x4004e6:   pinsrq xmm2,QWORD PTR [rsp+0x8],0x1
   0x4004ef:   pinsrq xmm2,QWORD PTR [rsp],0x0
   0x4004f7:   aesdec xmm2,xmm1
   0x4004fc:   comiss xmm2,xmm0
   0x4004ff:   jne    0x40053c
(gdb) 
```

Hey... we have a tool to deal with that!

Let's grab the values we need:
```
(gdb) b *0x4004e6

Breakpoint 6 at 0x4004e6
(gdb) c

Continuing.
ABCDEFGHIJKLMNO

Breakpoint 6, 0x00000000004004e6 in ?? ()

(gdb) info registers xmm0

xmm0  ...   uint128 = 0xd3fed9470883f49dd0c906f67f7f1915
(gdb) info registers xmm1

xmm1  ...   uint128 = 0x84c94ee22a094aa3245fdd370896258d
```

And plug them in:
```
$ ./inv_aesdec 84c94ee22a094aa3245fdd370896258d d3fed9470883f49dd0c906f67f7f1915
0x23e5a9dc8ebf0bd7c406bb5d2d1e12a1
a1 12 1e 2d 5d bb 06 c4 d7 0b bf 8e dc a9 e5 23
```

Then the same rigamarole as before:
```
(gdb) set {long}$rsp = 0xc406bb5d2d1e12a1
(gdb) set {long}($rsp+8) = 0x23e5a9dc8ebf0bd7
(gdb) x /16b $rsp

0x7fffffffe130:   0xa1  0x12  0x1e  0x2d  0x5d  0xbb  0x06  0xc4
0x7fffffffe138:   0xd7  0x0b  0xbf  0x8e  0xdc  0xa9  0xe5  0x23

(gdb) hbreak *0x400545

Hardware assisted breakpoint 7 at 0x400545
(gdb) c

Continuing.

Breakpoint 7, 0x0000000000400545 in ?? ()

(gdb)  x/10i $pc

=> 0x400545:   xor    rdi,rdi
   0x400548:   mov    rsi,rsp
   0x40054b:   mov    rdx,0x10
   0x400552:   xor    rax,rax
   0x400555:   syscall 
   0x400557:   ror    r11,0x20
   0x40055b:   mov    r11b,0xcf
   0x40055e:   push   rbx
   0x40055f:   pop    rbx
   0x400560:   ror    r9,0x30
```

# Automation

The fourth check goes back to aesdec, but the "comiss" comparison is at 40069d 
and the .text section extends to 466d97, so it's clear this could go on for a while.

As long as it sticks to the same format, we should be able to automate this.

The game plan is to write a Pin tool that looks for aesenc and aesdec instructions involved in the comparisons,
then calculates and patches in the correct input each time.

This tool can be found in tools/automated. 
Be sure to take a look at tools/automated/README.md for instructions on building and using it with Intel Pin.

Running the tool against the target we get:
```
pin/source/tools/solver$ ../../../pin -t obj-intel64/solver.so -o password.bin -- ./encrypted_box
[4001c9] "\x4e\xbd\x8e\x76\x27\xd6\x6f\x75\x62\xed\x5c\x13\x78\x2c\x93\x33"
[40035c] "\xbd\xd8\x8b\x80\xf7\x11\xdc\xd4\xb6\x8f\x95\x45\x42\xd9\xeb\x47"
[4004f7] "\xa1\x12\x1e\x2d\x5d\xbb\x06\xc4\xd7\x0b\xbf\x8e\xdc\xa9\xe5\x23"
[400698] "\x6d\x5c\x82\xca\x90\x91\xed\xcd\x24\x7d\x33\xf5\xbf\x59\x27\xd8"
...
```

We can see the flag printed out during the tool run, but the real test of course is to use the generated password file:
```
$ cat password.bin | ./encrypted_box
BRB{as_deep_as_OceanGate}
```

Nice!

This was a fun one that required learning more about the AES instruction set than I ever
imagined needing to.

It's worth taking a moment to appreciate that s4r also had to go through the same process
of finding the inverses in order to encrypt the program sections, then had to automate
the code generation as well.  Great challenge overall!

QED
