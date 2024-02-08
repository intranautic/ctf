Call gates are a means of transferring execution into a new code segment if protected mode is enabled, as within real mode there is only a flat memory space with no support for privilege levels.

You cannot modify the cs segment register in either modes, therefore you should perform a far call instruction. This involves the use of a far pointer, which is just another term for a logical address in the form of a segment selector paired with an offset ([[x86 Segmentation]]). In real mode, you by utilizing logical addressing you can modify the code segment register using the far variants of the call/jmp instructions, this is useful as a 16 bit segment selector can only address 64kb.

In protected mode, a call gate is a code descriptor entry within either the GDT or an LDT. It serves as a mechanism for modifying the cs register, effectively allowing for a potential privilege level switch of the currently running process. You issue a call instruction with a far pointer, the segment selector within the far pointer should point to a call gate entry within a descriptor table. 

The offset of the far pointer/logical address when performing a far call/jmp is not used, ie: `selector:offset`, only the `selector` is relevant but the `offset` is required for the processors logical addressing. The selector should point to a call gate within the descriptor table.

A call gate descriptor essentially functions as a far pointer as well to a code segment within a descriptor table. It contains a segment selector to the code segment, as well as offset to the base address of the code segment the call gate is pointing to. Like all segment descriptors on a descriptor table, if the system bit is cleared (4th bit of the access field) then the size of the entry is expanded from 8 to 16 bytes to allow for 64 bit addresses ([[x86 GDT and LDT]]).

An inter-privilege far call via a call gate pushes `ss:rsp` and `cs:rip` onto the stack, similar to a near call which saves the stack and instruction pointers for return. The return can then pop the necessary values back to restore the previous frame and execution state. The following is the 64 bit stack layout after a far call with a CPL change via a call gate.

```
+24: [ss]
+16: [rsp]
+8:  [cs]
0:   [rip]
```

The far return will therefore know to pop and restore the necessary values off the stack to return to the previous code segment and CPL.

Call gates are not typically used anymore due to security concerns, most operating systems now utilize interrupts ([[x86 Interrupts and Exceptions]]) to implement system calls, a means signaling to the kernel to perform operations on behalf of the user land process. However system calls can change CPL from user to kernel land, while call gates can modify the cs register and by product CPL arbitrarily.