In the previous section we talked about the format of the segment selector which is held within the segment register when performing logical addressing ([[x86 Segmentation]]). The selector is then utilized to resolve the descriptor from a [[x86 GDT and LDT]] entry as specified by the selector index into the segment registers descriptor cache.

Segment descriptors describe the memory segment referred to in the segment selector in the logical address ([[Address Space Terminology]]). It contains the segment base address, limit (denoting size of the segment), access permissions and control bits.

The 8 byte segment descriptor structure is as follows.

```
from: /arch/x86/include/asm/desc_defs.h#L16
struct desc_struct {
  u16	limit0;
  u16	base0;
  u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
  u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
} __attribute__((packed));
```

For the `cs` segment descriptor, its Descriptor Privilege Level is the cpus current Running Privilege Level. Due to this, the `cs` segment selector cannot be mutated like the rest of the segment registers due to this sensitive nature.

In order to modify the cs segment selector, you need to utilize either a `call far` instruction which makes use of a call gate ([[x86 Call Gates]]) on a descriptor table or an interrupt ([[x86 Interrupts and Exceptions]]).

A system descriptor is a special kind of segment descriptor, of which there are two kinds: system segment descriptors and gate descriptors. Gate descriptors are the aforementioned call gates, and system segment descriptors are used to describe the LDT and TSS segments ([[x86 Tasks and TSS]]).

System segment descriptors in long mode will have the S bit cleared to 0, with the descriptor being expanded to 16 bytes as opposed to the typical 8 byte width descriptors. This is to allow for the descriptors to hold 64 bit addresses for functionality such as long mode call gate descriptor which must contain a selector to a valid code segment on a descriptor table and a 64 bit address for the target code segment offset.
