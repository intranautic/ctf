There are multiple modes in which the processor can be in, each of which shown below.

* Real Mode
  * Runs in 16 bit mode
  * The original execution environment of the intel 8086 processor. Even modern processors all boot into real mode, the bootloader is responsible for enabling other modes.
  * There is no virtual memory or privilege rings present in this mode
  * When a reset operation occurs, the system will transfer back into real mode.
* Protected Mode
  * This is the mode most operating systems execute in due its support for virtual memory and privilege rings.
  * Virtual memory is a means of segregating each process into its own virtual memory space and allowing it to request/map physical pages into a virtual process. This means when a single process throws an exception or fails it wont bring the entire operating system down with it.
  * Privilege rings allow for a kernel operating in protected mode to segment the privileges into rings between the kernel and user modes. There are four levels however most operating systems only use rings 0 (kernel) and 4 (userspace). Rings 1 and 2 were originally designed for the operating system services.
* System Management Mode
  * An isolated and the most privileged mode designed to carry out privileged operations such as power management and system security. In this mode you have access to all physical resources including the entire address space.
  * The processor transfers to SMM whenever a system management interrupt (SMI) is invoked. Exit from SMM is achieved by executing the resume (RSM) instruction.
  * When entering SMM, the processor writes its current state to a designated address in memory called the SMRAM. The SMRAM is hidden and inaccessible from all other modes of execution.
  * The SMRAM is located in a reserved region of the systems physical memory and can be configured during boot process. This SMRAM cannot be rwx to unless the processor is in system mangement mode.
  * Firmware for the SMM is stored on the SPI chip on the motherboard, the BIOS will load the SMM into the designated region of memory designated for SMRAM. 
  * SMM firmware isnt actually stored on disk, so injecting our own code into SMM will persist even if disk is wiped. Need physical access to manually flash malicious code onto SPI however. Arch 4021 should go into this further.
* Long Mode
  * Since the processor always starts in real mode, on 64 bit machines the bootloader has to move the processor into long mode to utilize 64 bit.
  * Long mode consists of two sub modes, those being 64 bit and compatibility mode. Compatability mode is utilized for backwards compatibility with 32 and 16 bit programs. Each thread has its own processor state, when the kernel resumes a 16/32 bit task, it will enable compatibility mode.

When actually loading the processor into 64 bit mode, there are a few steps we have to take.

We start off in real mode, we first enable protected mode by setting CR0.PE=1. From here we can enable long mode, by setting the bits to EFER.LME=1, CR0.PG=1 and CR0.PAE=1. When in long mode its up to the kernel to switch between 64 bit and compatibility mode when executing legacy programs.
