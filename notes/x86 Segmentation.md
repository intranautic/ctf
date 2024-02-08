A mechanism for dividing the linear memory space into smaller protected address spaces known as segments. Due to the fact that real mode is always uses 16 bit addressing mode, segmentation provides a means of extending the addressable memory beyond the 64kb limit of a 16 bit address space.

For a reference on [[x86 Execution Modes]].

Segmentation works differently between real and protected mode, as the segment registers serve different purposes. There is no way to disable segmentation on 8086 processors.

Each segment register is 16 bits in length and its use depends on the mode of execution the processor is currently in. A logical address/far pointer consists of a segment selector and an offset.

In real mode, the segment register contains whats known as the segment selector. When addressing, the segment selector is shifted by 4 bits to produce a linear 20 bit address, of which the last 4 least significant bits are always zero. The segment address is then composed to a 16 bit offset (only 16 bit due to real mode) to yield a linear address which is the same as a physical address in real mode. The effective 20 bit linear address space within real mode is 2^20 bytes or approx 1mb.

In protected mode the segment selectors consists of an index, table indicator and a requested privilege level. The protected mode segment selector layout is shown below.

```
[     index     ][TI][RPL]
```

The index is 13 bits, TI 1 bit and RPL 2 bits contained within a 16 bit segment register.

In 64 bit mode segmentation is generally but not completely disabled. The processor treats the segment base of CS, DS, ES and SS as zero which creates a linear address that is equal to the effective address.

Each segment register contains whats known as a descriptor cache or a shadow register. When a selector is being loaded into a segment register from the [[x86 GDT and LDT]], the corresponding descriptor is loaded into the shadow/hidden portion of the segment register. This descriptor cache contains the entry of the GDT in which the selector points to; those being the base address, limit and access/permissions information for the segment.

More information on the format of the [[x86 Segment Descriptor]] structure will be detailed in the Global/Local Descriptor Table sections. A segment descriptor is a structure and an entry within these two tables.

The exception to this are the FS and GS segment registers which can be used for logical addressing in 64 bit mode. They facilitate addressing local data and certain operating system data structures. This does not apply to compatability mode, in which segmentation is still enabled.

In modern operating systems which run in 64 bit mode the FS/GS segment registers are typically utilized as the thread register. More specifically they point to a thread control block or [[Thread Local Storage]].

Due to this, the CS/SS/DS/ES segment registers all contain hardcoded base addresses and limits as they are not used in 64 bit protected mode. The hard coded base address is 0 and the limit is 2^64-1. The FS/GS registers however still abide by these rules and fills the descriptor cache from either the GDT/LDT.

When performing logical addressing in 64 bit protected mode, we have the similar layout of the far pointer which consists of the segment selector and the offset. Except as previously stated this selector is a structure which when resolved places descriptor information from either the GDT/LDT into the descriptor cache. From here the base address is taken from the resulting descriptor cache and added to the offset.

As previously shown the TI (Table Indicator) bit determines whether to use the GDT where TI=0 or LDT where TI=1. There is also the RPL (Requested Privilege Level), which is 2 bits wide. This field contains the privilege ring in which we want to perform the access in.

Using this the processor will perform a privilege check, `max(CPL, RPL) <= DPL`, where CPL (Current Privilege Level) is the lower 2 bits of the CS register and the DPL (Descriptor Privilege Level) which is the privilege level of the segment the descriptors base address is pointing to.

After resolving the logical address in protected mode, it results in a virtual linear address. This virtual address then needs to go through another process to be translated from virtual into a physical address.
