Over the years as more and more features have been composed on top of the 8086 processor family, there has been a need for more registers to hold information about hardware support and configuration.

MSRs provide a scaleable means of holding feature flags. Must be in kernel mode to access MSRs. The instructions to read/write to MSRs are RDMSR and WRMSR.

One such MSR we have already encountered is EFER, which stands for Extended Feature Enables. We had referred to it prior when setting up 64 bit mode. Using the CPUID instruction we can see whether or not the EFER MSR exists on the hardware.
