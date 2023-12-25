// parse and print the x64 segment selector and descriptors
// we cannot access descriptor cache, so in order to read we must manually
// index the GDT and parse each descriptor entry.
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");


typedef union selector {
  uint16_t data;
  // little endian format
  struct {
    uint16_t rpl: 2;
    uint16_t ti: 1;
    uint16_t index: 13;
  } __attribute__((__packed__));
} selec_t;

typedef struct descriptor {

} descr_t;

static void print_selec(const char* name, selec_t sel) {
  printk("%s: [IDX: %d][TI: %d][RPL: %d]\n",
    name,
    sel.index,
    sel.ti,
    sel.rpl
  );
  return;
}

// print a segment descriptor given selector and gdt ptr
static void print_desc(struct desc_ptr* gdt) {
  /* from: /arch/x86/include/asm/desc_defs.h#L16
  struct desc_struct {
    u16	limit0;
    u16	base0;
    u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
    u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
  } __attribute__((packed));
  */

}

static int __init segments_init(void) {
  printk("[segments]: kernel module loaded\n");
  // since x64 bit mode only cs, ss
  selec_t cs, ss;
  asm volatile (
    ".intel_syntax noprefix;"
    "mov %0, cs;"
    "mov %1, ss;"
    ".att_syntax;"
    : "=r" (cs.data),
      "=r" (ss.data)
  );
  print_selec("cs", cs);
  print_selec("ss", ss);

  // kernel provides headers for gdt ptr and desc format
  struct desc_ptr gdt;
  // implicit store on stack, uses att syntax for rsp/rbp
  // so use att_syntax default
  asm volatile (
    "sgdt %0;"
    : "=m" (gdt)
  );
  printk("GDT Base: %#lx\n", gdt.address);
  printk("GDT Limit: %d\n", gdt.size);
  return 0;
}

static void __exit segments_exit(void) {
  printk("[segments]: kernel module unloaded\n");
}

module_init(segments_init);
module_exit(segments_exit);
