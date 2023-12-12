# fsb

i fucking hate fsb, dumb idiot stupid bug class will only learn for ctf lmfao.

We can use fmt strings to leak addresses on the stack (blah blah blah we all know), if we control the format string then we can specify an arbitrary amount of fmt strings (within the bounds of our buffer) to read the stack.

```c
%[parameter][flags][width][.precision][length]format
```

The `%n` format specifier writes the amount of bytes written to stdout into a pointer. This is useful in the following scenario.

```c
int main(int argc, char** argv) {
  int len;
  printf("AAAA%n", &len);
  printf("%d\n", len); // output: 4
}
```

It will write `4` to `len`, which is the amount of bytes written so far. We can utilize this to gain arbitrary writes on pointers on the stack via utilizing the positional specifier `$`.

In our format string we could include pointers to memory addresses we want to write to and use the `%n` fmt string to write to that address, giving us an arbitrary write primitive.

We can also use length specifiers in our format string to specify the length of our operation.

There are two ways of controlling the value to write to the stack, first we can literally write padding into our format string like so: `AAAA%10$n`, which will write `0x4` (len of current output) to the pointer at the 10th index on the stack.

Or we can specify padding via another format string, in this case as shown as follows: `%500x%10$n`. This will write out `0*500` as padding, then the %n format string which essentially allows us to specify a value without ever having to manually write it out.

Keep in mind we are usually working with constrained environments so a buffer the size of an address we would like to write isnt ever really a possiblity.

However, there is a long time we have to wait when using padding of that length. There is another way to write arbirary values to pointers without the use of such large padding, and that is to use `%hhn`, which lets use write character by character. The caveat here is that we can only use this for values which are sequential, meaning if the first byte is `1` then the next has to be over the value of one. We can write at different offsets in different order to gain arbitrary writes using `%hn`.

By using the `*` in the format string we can specify a dynamic padding size, example: `%*c` will read the first parameter as the length of the padding. We can also use positional specifiers `%*10$c`, it will read in the 10th argument on the stack and use that as the padding length.

We can copy and write from arbitrary memory addresses in a single printf using this technique. We can use that value as a dynamic padding size when performing an arbitrary write, so we can write that value to another arbitrary point in memory.

If PIC isnt enabled for the binary then we can overwrite the GOT directly, allowing us to hijack control flow.

We can also hijack the return address with a single format string vulnerability. Due to the fact that we only have values present on the stack, we have access to saved base pointers (but for some reason no return addrs?).

Using this base pointer relative to our fmt ptr on the stack, we can perform a partial overwrite on the base ptr to the return address. In doing so we gain a pointer to a saved return address on the stack, with that pointer we can overwrite the ret addr to whatever value we want.

Another technique is to leak a value on the stack near address of the saved return address. We can perform a partial overwrite to get a pointer to the stack address, which we can utilize to hijack the instruction pointer.

Keep in mind, we can either perform a partial overwrite an address on the stack to point to the relative return address (as its unaffected by ASLR). Or we can leak that address and manually adjust the offset, then write to that saved return address. We can also ROP using this, since we have arbitrary write and we know exactly where the stack address is.
