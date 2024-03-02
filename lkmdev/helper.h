#ifndef HELPER_H_
#define HELPER_H_

#define INIT_WRAP(sym, func) \
  static int __init sym(void) {func} \
  module_init(sym);

#define EXIT_WRAP(sym, func) \
  static void __exit sym(void) {func} \
  module_exit(sym);

#endif // HELPER_H_
