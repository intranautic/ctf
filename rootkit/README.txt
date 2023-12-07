a study and reference for lkm rootkit techniques

syscall_hook.c      - basic sys_call_table hooking demonstration.
inline_hook.c       - inline hooking demonstration of sys_call_table.
kmem_hook.c         - writing to kernel memory via /dev/kmem technique.
idt_hook.c          - evasive IDT hooking technique demonstration.
pgfault_hook.c      - linux page fault handler hijacking technique demonstration.
fileop_hijack.c     - hijacking the file_operation structure demonstration.
syshandler_hijack.c - directly patching the syscall handler technique.
ftrace_hook.c       - kernel function hooking via linux ftrace feature.
ebpfrk.c            - poc rootkit utilizing linux ebpf features.
hiderk.c            - hiding lkm modules, vfs, network and processes.
keylog.c            - basic lkm rootkit keylogger demonstration.

references:

