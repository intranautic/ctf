// gcc -shared -fPIC -o libgcc_s.so.1 stub.c -Wl,--version-script=stub.map
void _Unwind_Resume(void) { __builtin_trap(); }
void __gcc_personality_v0(void) { __builtin_trap(); }
