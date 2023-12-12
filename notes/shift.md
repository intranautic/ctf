# shift

In some cases when we dont have a buffer overflow but some mechanism to hijack control flow we can utilize a singular gadget to shift the stack pointer into a user controlled buffer. From there we can utilize ROP.

An example of this is with a fsb, if we can hijack program control via overwriting something like the GOT then we can perform this technique. In most cases there wont really be a gadget which shifts the stack pointer by a large offset present within the binary, but exists in libc.
