// gcc -w -static -no-pie -fno-stack-protector -o vuln vuln.c
#include <stdio.h>
#include <unistd.h>

int foo(char* buffer) {
  return read(0, buffer, 1024);
}

int main(int argc, char** argv) {
  char buffer[128];
  printf("Bytes read into buffer: %d\n", foo((char *)&buffer));
  puts("Good luck on calling /bin/sh");
  return 0;
}
