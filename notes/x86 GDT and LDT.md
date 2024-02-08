## Global/Local Descriptor Tables
The GDT/LDT are data structures which are used in every mode of execution other than real mode due to its simplified method of segmentation which was previously shown.

First, the GDT is a table of 8 byte entries which primarily contains segment descriptors, but can also contain task state segments, the Local Descriptor Table or a call gate. Each core contains its own state, including its own set of registers. The special register which contains information on the GDT in memory is the GDTR register which stands for Global Descriptor Table Register.

```
GDTR: [32/64 bit linear base address][16 bit table limit]
```

The register is 10 bytes (64 + 16 bits) on 64 bit systems and 6 bytes (32 + 16 bits) on 32 bit systems. The upper 64/32 bits contains the base address of the Global Descriptor Table, and since the GDT is only utilized in protected mode its base is a virtual address. The lower 16 bits serve as the table limit, which specifies the size of the table in bytes, with (base address + limit) being the address of the last inclusive byte of the table.

To operate on this register we can use the LGDT and SGDT instructions to load/store values in the GDTR register.

The LDT performs a similar function to the GDT in which it contains segment descriptors. To address the LDT you need to go through the GDT. Due to this, the LDTR register is similar to the other segment registers, as it contains a 16 bit selector as an index into the GDT and a hidden region of the register for the descriptor cache. The instructions LLDT and SLDT load and store 16 bits into the LDTR register respectively.

The TI bit of the LLDR must be set to zero to index the GDT, since the Local Descriptor Table cannot contain nested LDTs.

The LDTs purpose was for systems which used [[x86 Segmentation]] as opposed to [[x86 Paging]]. In such cases, each process will be assigned its own unique LDT and each context switch will simply modify the LDTR to point to the correct descriptor on the GDT. With this each process will have its own unique set of segments which prevents them from accessing other processes memory. Its an archaic mechanism for process isolation and memory protection on 32 bit protected mode.

Both these tables are populated with segment descriptors ([[x86 Segment Descriptor]]). These tables are only enabled within protected mode, the segment registers serve different purposes within real mode as documented within the segmentation entry.