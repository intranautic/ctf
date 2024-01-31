// helper for heap allocation
#ifndef ALLOC_H_
#define ALLOC_H_
// can be redefined at compile time. careful, this is very hacky
#define HEAP_ALLOCATOR(sz) calloc(1, sz)
#define _MEMDUP(src, size) memcpy(HEAP_ALLOCATOR(size), src, size)
#define HEAP_ALLOC(T, ...) \
  _MEMDUP(((T []){__VA_ARGS__}), sizeof((T []){__VA_ARGS__}))
#endif // ALLOC_H_
