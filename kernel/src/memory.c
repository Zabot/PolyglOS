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
  // TODO Flatten the memory map list so we don't have to deal with overlapping
  //      regions elsewhere.
  for (int i = 0; i < mapLength; i++) {
    if (memoryMap[i].baseAddress + memoryMap[i].length > max)
      max = memoryMap[i].baseAddress + memoryMap[i].length;
  }
  frames = max / PAGE_SIZE;
  printf("%d memory frames detected\n", frames);

  // In order to bootstrap memory managment we need to slap this bitmap
  // somewhere. Find a suitable chunk of memory in the memory map
  int bitmap_size = frames / WORD_SIZE;
  for (int i = 0; i < mapLength; i++) {
    if (memoryMap[i].type == BIOS_MEMORY_TYPE_USABLE) {
      if (memoryMap[i].length > bitmap_size) {
        // Set framesInUse to the very end of the last addressable region. Using
        // the end means we don't have to worry about clobbering anything the
        // BIOS may have set up in low memory.
        framesInUse = (void *)(WORD)(memoryMap[i].baseAddress + memoryMap[i].length - bitmap_size);
      }
    }
  }

  printf("Storing memory bitmap at %x\n", framesInUse);

  // The frames bitmap is initalized to have no unused frames. Then we loop
  // through the memory map to free an usable frames
  printf("Initalizing memory bitmap...\n");
  setBitmap(framesInUse, 0, frames, 1);
  printf("Marking free memory frames...\n");
  for (int i = 0; i < mapLength; i++) {
    // Since regions may start and stop off of frame borders, we need to calculate
    // the start and stop frame of each region.
    int baseFrame = memoryMap[i].baseAddress / PAGE_SIZE;
    int endFrame = (memoryMap[i].baseAddress + memoryMap[i].length) / PAGE_SIZE;
    int length = endFrame - baseFrame;

    // We add an additional frame of padding because the base frame is rounded
    // down, and may therefore contain unusable memory
    if (memoryMap[i].type == BIOS_MEMORY_TYPE_USABLE) {
      printf("Freeing frames %x to %x...\n", baseFrame, endFrame);
      setBitmap(framesInUse, baseFrame + 1, length - 1, 0);
    }
  }

  // Allocate pages for memory bitmap
  int memoryMapStartFrame = (int)framesInUse / PAGE_SIZE;
  int memoryMapFrames  = (bitmap_size / PAGE_SIZE + 1);
  printf("Reserving frames %x to %x for memory bitmap...\n", memoryMapStartFrame, memoryMapFrames);
  setBitmap(framesInUse, memoryMapStartFrame, memoryMapFrames, 1);

  // TODO Allocate pages in use by the kernel

  // The memory bitmap now resides in memory at the top of the address space
  // and is initalized with only the frames reserved by the BIOS, and the frames
  // containing the memory bitmap itself.
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
