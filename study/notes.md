# random pwn notes

random notes for pwn

## gadget fun

didnt realize this, but we can control the ax register via calling other functions
since the return value is passed through ax. for example we can read() n bytes to whatever
value we want to set for rax and use it for a system call. we can also perform partial
overwrites on GOT entries for some gadgets in libc. in pwnable.tw unexploitable, we can
overwrite the lsb byte on the GOT entry for sleep to execute a syscall instruction as
there isnt one present in the binary.

alignment also matters, in the case of pwnable.kr's unexploitable, we can execute data
as a system call instruction in the following code:

```asm
┌ 52: int main (int argc, char **argv, char **envp);
│           ; var void *buf @ rbp-0x10
│           0x00400544      55             push rbp
│           0x00400545      4889e5         mov rbp, rsp
│           0x00400548      4883ec10       sub rsp, 0x10
│           0x0040054c      bf03000000     mov edi, 3                  ; int s
│           0x00400551      b800000000     mov eax, 0
│           0x00400556      e8f5feffff     call sym.imp.sleep          ; int sleep(int s)
│           0x0040055b      488d45f0       lea rax, [buf]
│           0x0040055f      ba0f050000     mov edx, 0x50f              ; 1295 ; size_t nbyte
│           0x00400564      4889c6         mov rsi, rax                ; void *buf
│           0x00400567      bf00000000     mov edi, 0                  ; int fildes
│           0x0040056c      b800000000     mov eax, 0
│           0x00400571      e8bafeffff     call sym.imp.read           ; ssize_t read(int fildes, void *buf, size_t nbyte)
│           0x00400576      c9             leave
└           0x00400577      c3             ret
```

`0x50f` in little endian format is `0x0f05`, which is the syscall instruction. We can use
this as a gadget as it does not check for alignment.

## null byte string check

we can bypass or mess with various string functions by passing a null byte.
