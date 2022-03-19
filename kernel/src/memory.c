#include <stdint.h>

#include "io/format.h"

#include "memory.h"

#define VIRTUAL_MEMORY_SIZE 0xFFFFFFFF
#define PAGE_SIZE 4096
#define PAGE_TABLE_DEPTH 2

/*Type 1: Usable (normal) RAM*/
/*Type 2: Reserved - unusable*/
/*Type 3: ACPI reclaimable memory*/
/*Type 4: ACPI NVS memory*/
/*Type 5: Area containing bad memory */
struct __attribute__((__packed__)) MemoryMapEntry {
  uint64_t baseAddress;
  uint64_t length;
  uint32_t type;
  uint32_t acpiAttributes;
};

// These are populated by the bootloader prior to launching the kernel
extern uint32_t memoryMapLength;
extern struct MemoryMapEntry memoryMap[];

void printMemory() {
  int mapLength = (memoryMapLength - (uint32_t)(&memoryMap))/24;
  printf("System memory map\nStart:Length (Type)\n----------------\n");
  for (int i = 0; i < mapLength; i++) {
    printf("%x:%x (%x)\n", memoryMap[i].baseAddress, memoryMap[i].length, memoryMap[i].type);
  }
}
