#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NOTE_NUM  4
#define NOTE_SIZE 0x800

void print(const char *s) {
  if (write(STDOUT_FILENO, s, strlen(s)) <= 0)
    _exit(1);
}

void getstr(const char *s, char *buf, size_t len) {
  print(s);

  for (size_t i = 0; ; i++) {
    char c;
    if (read(STDIN_FILENO, &c, 1) <= 0)
      _exit(1);
    else if (c == '\n') {
      buf[i] = '\0';
      break;
    } else if (i < len) {
      buf[i] = c;
    }
  }
}

int getint(const char *s) {
  char buf[0x8] = { 0 };
  getstr(s, buf, sizeof(buf) - 1);
  return atoi(buf);
}

void main(void) {
  char *note_list[NOTE_NUM] = { NULL };
  print("--- Himitsu Note ---\n"
        "1. add\n"
        "2. edit\n");

  while (1) {
    int choice = getint("> ");
    switch (choice) {
      case 1: {
        // unsigned so no negative oob indexing
        unsigned int i = getint("index: ");
        // no bounds checking, we can overwrite pointers on the stack as long as they are nil
        // unlimited allocations as long as we have negative values on the stack, we should
        // have this the farther up the stack we go into auxv
        if (!note_list[i]) {
          note_list[i] = (char*)malloc(NOTE_SIZE);
          print("[+] done\n");
        } else {
          print("[-] error\n");
        }
        break;
      }
      case 2: {
        // holy shit i can overwrite the stack pointer i forgot
        // we can write to pointers regardless of whether its allocated or not
        unsigned int i = getint("index: ");
        getstr("data: ", note_list[i], NOTE_SIZE - 1);
        break;
      }
      default: {
        print("[+] bye\n");
        for (int i = 0; i < NOTE_NUM; i++)
          if (note_list[i])
            free(note_list[i]);
        memset(note_list, 0, sizeof(note_list));
        return;
      }
    }
  }
}
