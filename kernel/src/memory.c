#include "memory.h"

#include <stdint.h>

#include "io/format.h"
#include "bitmap.h"

#define BIOS_MEMORY_TYPE_USABLE 1

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
  printf("System memory map\nStart:Length (Type)\n----------------\n");
  int mapLength = (memoryMapLength - (uint32_t)(&memoryMap))/24;
  for (int i = 0; i < mapLength; i++) {
    printf("%l:%l (%d)\n", memoryMap[i].baseAddress, memoryMap[i].baseAddress + memoryMap[i].length - 1, memoryMap[i].type);
  }
}

int frames;
WORD *framesInUse;
void initalizeFrameBitmap() {
  // Determine the number of memory frames
  uint64_t max = 0;
  int mapLength = (memoryMapLength - (uint32_t)(&memoryMap))/24;
  for (int i = 0; i < mapLength; i++) {
    if (memoryMap[i].baseAddress + memoryMap[i].length > max)
      max = memoryMap[i].baseAddress + memoryMap[i].length;
  }
  frames = max / PAGE_SIZE;

  // In order to bootstrap memory managment we need to slap this bitmap
  // somewhere. Find a suitable chunk of memory
  int bitmap_size = frames / WORD_SIZE;
  // TODO Read through memory map looking for something that has enough memory
  // Assign a voidpointer to framesInUse
  framesInUse = (void *)(WORD)memoryMap[3].baseAddress;

  // The frames bitmap is initalized to have no unused frames. Then we loop
  // through the memory map twice. The first time is to free any frames that the
  // memory map reports are usable. The second time is to handle the case of
  // overlapping entries allowing some unusable memory to appear usable by
  // explicitly removing any memory that is unusable.
  // TODO We need to put this somewhere
  setBitmap(framesInUse, 0, frames, 1);
  for (int b = 0; b < 2; b++) {
    for (int i = 0; i < mapLength; i++) {
      int baseFrame = memoryMap[i].baseAddress / PAGE_SIZE;
      int endFrame = (memoryMap[i].baseAddress + memoryMap[i].length) / PAGE_SIZE;
      int length = endFrame - baseFrame + 1;

      if (b == 0 && memoryMap[i].type == BIOS_MEMORY_TYPE_USABLE)
        setBitmap(framesInUse, baseFrame, length, 0);
      if (b == 1 && memoryMap[i].type != BIOS_MEMORY_TYPE_USABLE)
        setBitmap(framesInUse, baseFrame, length, 1);
    }
  }
}

int getFrame() {
  for (int f = 0; f < frames; f++) {
    if (getBitmap(framesInUse, f) == 0) {
      setBitmap(framesInUse, f, 1, 1);
      return f;
    }
  }

  return -1;
}

void printFrames() {
  for (int f = 0; f < frames; f++) {
    printf("%d", getBitmap(framesInUse, f));
  }
  printf("\n");
  printf("Dumped %d frames\n", frames);
}
