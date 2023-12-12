The Global Offset Table and Procedure Linkage Table are two structures responsible for dynamic linking and lazy import resolution at runtime.

When designing an executable, there are a few things we have to consider. First and foremost is about how we can reduce the actual size of the executable. Well most binaries especially those which run on top of a c runtime provided by the system libc depend on external shared libraries.

So static linking will resolve these relocations by resolving the symbols, assigning static addresses (others are resolved via relative addressing), then merging the text and data sections together into the resulting binary.

However dynamic linking is managed by the loader or the dynamic linker. This process is done at runtime and can be performed lazily given some conditions are met. One such condition is that full RELRO (RELocation Read Only) is not enabled, which essentially dynamically populates the entire GOT at startup then sets that page to read only.

This prevents GOT overwrite attacks which allow us to hijack program control, common technique which utilizes this is Format String Bug but any arbitrary write can achieve this given that the address of the GOT table is known at runtime.

There are three sections responsible for this, those being the `.got`, `.got.plt` and `.plt` sections.

The `.got` section contains init/fini functions for csu, essentially relocations to code which must be resolved at startup.

```
.got:0000000000003FC0 ; ===========================================================================
.got:0000000000003FC0
.got:0000000000003FC0 ; Segment type: Pure data
.got:0000000000003FC0 ; Segment permissions: Read/Write
.got:0000000000003FC0 _got            segment qword public 'DATA' use64
.got:0000000000003FC0                 assume cs:_got
.got:0000000000003FC0                 ;org 3FC0h
.got:0000000000003FC0 __libc_start_main_ptr dq offset __libc_start_main
.got:0000000000003FC0                                         ; DATA XREF: _start+1F↑r
.got:0000000000003FC8 _ITM_deregisterTMCloneTable_ptr dq offset _ITM_deregisterTMCloneTable
.got:0000000000003FC8                                         ; DATA XREF: sub_1070+13↑r
.got:0000000000003FD0 __gmon_start___ptr dq offset __gmon_start__
.got:0000000000003FD0                                         ; DATA XREF: _init_proc+8↑r
.got:0000000000003FD8 _ITM_registerTMCloneTable_ptr dq offset _ITM_registerTMCloneTable
.got:0000000000003FD8                                         ; DATA XREF: sub_10A0+24↑r
.got:0000000000003FE0 __cxa_finalize_ptr dq offset __cxa_finalize
.got:0000000000003FE0                                         ; DATA XREF: sub_10E0+E↑r
.got:0000000000003FE0                                         ; sub_10E0+22↑r
.got:0000000000003FE0 _got            ends
.got:0000000000003FE0
```

The `.got.plt` section is where all imports which can be populated lazily are stored, along with reserved entries for special structures and the resolved address of dl_runtime_resolve. `.got.plt[0]` is reserved for a pointer to the `link_map` structure in `ld.so` and `.got.plt[1]` is reserved for dl_runtime_resolve.

Last the `.plt` section is utilized for stub, contains thunk/trampoline to either call resolve or the populated entry in .got.plt.

Simplified map as example is shown below.
```
call puts@plt

section..plt
  push section..got.plt
  jmp dl_runtime_resolve@got.plt
  puts@plt:
    jmp puts@got.plt
    push 0           ; got entry offset
    jmp section..plt
.section..got.plt
  dq struct link_map@ld.so
  dq dl_runtime_resolve@libc.so
  ; got entry by default points back to
  ; puts@got.plt -> jmp section..plt
  dq puts@plt
.section..got
  dq
  ... ; init funcs for csu resolved at startup
  dq
```
