/*
 * Malloc Playground
 * TODO
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <malloc.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#define ALLOC(type, ...) \
  (type *)memdup((type []){__VA_ARGS__}, sizeof(type))

typedef struct {
  pid_t pid;
  heap_env* next;
} heap_env;

typedef struct {
  heap_env* environment;
} playground;

int main(int argc, char** argv) {


  return 0;
}

