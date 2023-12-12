# ghost party notes

when passing/returning by value, it will invoke the copy constructor. this is why the rule of three five zero is so important for managing raw resources. if an explicit copy constructor is not defined (like in the challenge), it will default to an implicit shallow copy.

this leads to a uaf bug when passing by value as the copy constructor is invoked.

this vulnerability is very related to Rule of Three Five Zero as this rule serves to prevent such occurrences. keep in mind, this wont occur for instances in which Copy Elision (C++17) can be applied as the implicity copy constructor will never be invoked.

thanks to this uaf we can leak values on the heap by reading the the previously freed chunk.

since this is glibc 2.23 getting a shell with a leak and a double free is easy, we can do a fastbin dup to overwrite `__malloc_hook` with a one gadget and gain a shell.

another way is to hijack control of the vtable and overwrite an entry with a one gadget. either way we only need one mechanism to hijack control flow to pop a shell.

something important to note is the fact that for some strange, reason, uaf doesnt work when allocating higher addresses it gets split into smaller buffers i think? not too sure but ill look into `std::basic_string` to see if some c++ voodoo trickery is involved.

also since vtables are allocated on the heap lazily we can use the uaf to get a pointer to another class's virtual table. we dont have an edit function so we cant directly edit it, but we can leak it and if we gain a write what where vuln we can overwrite the vtable. we could also leak base address of binary with this since PIE is enabled.

