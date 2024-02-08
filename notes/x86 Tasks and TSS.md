Tasks are a fairly legacy feature due to low adoption by various operating systems for portability reasons (as this is an x86 specific feature). However, the TSS is still required and utilized on x86 based systems so it has to be setup regardless.

The TSS is a segment descriptor which is located on the GDT, there may be as many task state segments as the processor count allows (number of TS selectors). The TSS is loaded and accessed via the TR (Task Register) segment selector, which indexes a valid TSS on the GDT.

It can be loaded and stored via the LTR/STR instructions respectively, the TSS may reside anywhere in memory but should only be accessible to ring0 for obvious reasons.

In 32 bit protected mode, the TSS was originally designed for handling multiprocessing systems, so a single TSS consisted of the processors general purpose registers, segment registers, control pointer / flags,

TSS.rsp0 in long mode will contain the kernel stack address for a process which has been interrupted by an external event, since 64 bit fast syscall implementation relies on the gs base for store/restoring the kernel stack within an interrupt routine