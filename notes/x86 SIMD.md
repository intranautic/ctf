An in depth collection of processor extensions and lower level features, basic ISA and assembly not included since we know that by heart.
## x87 Instruction Set
This is an extension of the x86 instruction set utilized to operate with the auxiliary fpu unit. This instruction set has largely been superseeded by the sse and avx instruction sets and remains only for legacy purposes.

Not going to cover this since its literally obselete and im not particularly interested lol.
## x86 SIMD Extensions
The extended instruction sets are listed below. Keep in mind that not all modern processors support the newer extensions.

* SSE (Streaming SIMD Instructions)
* AVX/AVX512 (Advanced Vector Extensions)
* MMX (no meaning)?

The MMX instruction set was the first simd extension for intel processors, it is largely obselete now and has been replaced by SSE.

Each extension contains its own set of specialized registers, each are listed down below.

* MMX: mm0-mm7
  * a total of 8, 32 bit registers with a prefix of mm
* SSE: xmm0-xmm15
  * in 64 bit mode there are a total of 16, 128 bit registers
  * in 32 bit there are only 8 registers
* AVX: ymm0-ymm15
  * 16 registers, each 256 bits wide
* AVX512: zmm0-zmm31
  * 32 registers, each 512 bits wide
  * if the processor supports AVX512, then there will be 32 SSE, AVX and AVX512 registers available.

The 80 bit x87 auxiliary fpu registers are aliased to these registers as well, being overlaid on the 64 bit mmx registers.

If AVX512 is enabled, then the SSE, AVX and AVX512 registers are actually all aliased to each other; meaning that they refer to the same register under different names and sizes.

As previously shown, SIMD introduced a new set of registers as well as instructions to x86, which allows for specialized operations. 

This achieves parallelism by packing multiple elements of data into a single register then performing operations on the whole. Each extension generally follows this concept, logically segmenting the register to fit multiple elements then performing a singular operation on multiple data.

Here is an example of how multiple elements can be packed into a singular register. As an example we can utilize a register which is 512 bits wide.

* 64 bytes     (each 8 bit)
* 32 words    (each 16 bit)
* 16 dwords  (each 32 bit)
* 8 qwords    (each 64 bit)
* 16 single precision floating point (each 32 bit)
* 8 double precision floating point (each 64 bit)

By logically allocating the register into different sizes of data we can achieve vector paralellism. Data must be properly aligned, it is up to the compiler and the programmer to ensure that all data is same.

There is no lower level mechanism but there is type checking involved at the compiler level thanks to [[Compiler Intrinsics]].
