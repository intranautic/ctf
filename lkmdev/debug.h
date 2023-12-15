#ifndef DEBUG_H_
#define DEBUG_H_
#define dbg_info(fmt, ...) \
  do { \
    printk("%s:%d:%s(): " fmt "\n", \
      __FILE_NAME__, \
      __LINE__, \
      __func__, \
      ##__VA_ARGS__); \
  } while (0)
#define dbg_error(fmt, ...) \
  do { \
    dbg_info(fmt, ##__VA_ARGS__); \
    return -1; \
  } while (0)
#endif // DEBUG_H_
