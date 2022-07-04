#include "gdt/gdt.h"

#include <stdint.h>

#include "io/log.h"
#include "memory/heap.h"

#define GDT_FLAGS_PAGE_GRANULARITY 0x80
#define GDT_FLAGS_BIG 0x40
#define GDT_FLAGS_LONG_MODE_CODE 0x20

#define GDT_ACCESS_PRESENT 0x80
#define GDT_ACCESS_RING_3 0x60
#define GDT_ACCESS_RING_0 0x00
#define GDT_ACCESS_CODE_DATA 0x10
#define GDT_ACCESS_EXECUTABLE 0x08
#define GDT_ACCESS_RW_ENABLE 0x02

#define GDT_ACCESS_CODE_SEGMENT GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE
#define GDT_ACCESS_DATA_SEGMENT GDT_ACCESS_CODE_DATA

#define GDT_ACCESS_LDT                 0x02
#define GDT_ACCESS_TSS_16BIT_AVAILABLE 0x01
#define GDT_ACCESS_TSS_16BIT_BUSY      0x03
#define GDT_ACCESS_TSS_32BIT_AVAILABLE 0x09
#define GDT_ACCESS_TSS_32BIT_BUSY      0x0B

#define GDT_INDEX_KERNEL_CODE 1
#define GDT_INDEX_KERNEL_DATA 2
#define GDT_INDEX_USER_CODE 3
#define GDT_INDEX_USER_DATA 4
#define GDT_INDEX_TSS_BASE 5

struct __attribute__((packed)) gdt_descriptor {
  uint16_t size;
  uint32_t offset;
};

struct __attribute__((packed)) tss {
	uint32_t unused;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t _unused[22];
};

struct __attribute__((packed)) gdt_entry {
  uint16_t limitLow;
  uint16_t baseLow;
  uint8_t baseMid;
  uint8_t access;
  uint8_t flagsLimit;
  uint8_t baseHigh;
};

void load_gdt(struct gdt_descriptor*);
void load_tss(uint32_t gdt_offset);

void initalizeGDT(int cpus) {
  // 1 null segment
  // 2 kernel mode segments
  // 2 user mode segments
  // 1 tss for each cpu
  int gdtEntries = 5 + cpus;

  // Allocate the gdt table
  struct gdt_entry *gdt = malloc(gdtEntries * sizeof(struct gdt_entry));

  // Initalize each entry to a segment spanning all 4GB. The segment type
  // flags in `access` are unset and must be initalized manually. (Starting at
  // 1 to skip the null entry)
  for (int i = 1; i < gdtEntries; i++) {
    gdt[i].limitLow   = 0xFFFF;
    gdt[i].flagsLimit =   0x0F;

    gdt[i].baseLow  = 0x0000;
    gdt[i].baseMid  =   0x00;
    gdt[i].baseHigh =   0x00;

    gdt[i].flagsLimit |= GDT_FLAGS_PAGE_GRANULARITY | GDT_FLAGS_BIG;
  }

  // Kernel mode code and data
  gdt[GDT_INDEX_KERNEL_CODE].access =
    GDT_ACCESS_PRESENT
    | GDT_ACCESS_CODE_SEGMENT
    | GDT_ACCESS_RW_ENABLE;

  gdt[GDT_INDEX_KERNEL_DATA].access =
    GDT_ACCESS_PRESENT
    | GDT_ACCESS_DATA_SEGMENT
    | GDT_ACCESS_RW_ENABLE;

  // User mode code and data
  gdt[GDT_INDEX_USER_CODE].access =
    GDT_ACCESS_PRESENT
    | GDT_ACCESS_RING_3
    | GDT_ACCESS_CODE_SEGMENT
    | GDT_ACCESS_RW_ENABLE;

  gdt[GDT_INDEX_USER_DATA].access =
    GDT_ACCESS_PRESENT
    | GDT_ACCESS_RING_3
    | GDT_ACCESS_DATA_SEGMENT
    | GDT_ACCESS_RW_ENABLE;

  // Each CPU gets its own stack in the kernel
  for (int i = GDT_INDEX_TSS_BASE; i < gdtEntries; i++) {
    // Create a TSS that puts the stack in a block of memory in the kernel mode
    // data segment
    struct tss *t = malloc(sizeof(struct tss));
    t->ss0 = GDT_INDEX_KERNEL_DATA * sizeof(struct gdt_entry);
    t->esp0 = (uintptr_t)malloc(4096);
    t->unused = 0;
    for (int b = 0; b < 22; b++) {
      t->_unused[b] = 0;
    }

    // Add the TSS segment to the GDT
    uint32_t limit = sizeof(struct tss);
    gdt[i].baseLow  = (0x0000FFFF & (uintptr_t)t) >> 0;
    gdt[i].baseMid  = (0x00FF0000 & (uintptr_t)t) >> 16;
    gdt[i].baseHigh = (0xFF000000 & (uintptr_t)t) >> 24;

    gdt[i].limitLow   = (0x0FFFF & limit) >> 0;
    gdt[i].flagsLimit = (0xF0000 & limit) >> 16;

    gdt[i].flagsLimit |= GDT_FLAGS_BIG;
    gdt[i].access = GDT_ACCESS_PRESENT | GDT_ACCESS_TSS_32BIT_AVAILABLE;
  }

  // Prepare a GDT descriptor to pass to the lgdt instruction
  struct gdt_descriptor *descriptor = malloc(sizeof(struct gdt_descriptor));
  descriptor->offset = (uintptr_t)gdt;
  descriptor->size = gdtEntries * sizeof(struct gdt_entry) - 1;

  // Load all of the new tables
  load_gdt(descriptor);
  for (int i = 0; i < cpus; i++) {
    // TODO This doesn't actually work with more then one cpu since each load
    // happens on the same CPU.
    load_tss((GDT_INDEX_TSS_BASE + i) * sizeof(struct gdt_entry));
  }
}
