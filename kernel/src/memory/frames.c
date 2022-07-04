#include "memory/frames.h"

#include <stdint.h>
#include <stddef.h>

#include "io/log.h"
#include "bitmap.h"
#include "config.h"

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
  INFO("System memory map");
  INFO("Start:Length (Type)");
  INFO("----------------");
  int mapLength = (memoryMapLength - (uint32_t)(&memoryMap))/24;
  for (int i = 0; i < mapLength; i++) {
    INFO("%l:%l (%d)", memoryMap[i].baseAddress, memoryMap[i].baseAddress + memoryMap[i].length - 1, memoryMap[i].type);
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
  INFO("%d memory frames detected", frames);

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

  INFO("Storing memory bitmap at %x", framesInUse);

  // The frames bitmap is initalized to have no unused frames. Then we loop
  // through the memory map to free an usable frames
  INFO("Initalizing memory bitmap...");
  setBitmap(framesInUse, 0, frames, 1);
  INFO("Marking free memory frames...");
  for (int i = 0; i < mapLength; i++) {
    // Since regions may start and stop off of frame borders, we need to calculate
    // the start and stop frame of each region.
    int baseFrame = memoryMap[i].baseAddress / PAGE_SIZE;
    int endFrame = (memoryMap[i].baseAddress + memoryMap[i].length) / PAGE_SIZE;
    int length = endFrame - baseFrame;

    // We add an additional frame of padding because the base frame is rounded
    // down, and may therefore contain unusable memory
    if (memoryMap[i].type == BIOS_MEMORY_TYPE_USABLE) {
      INFO("Freeing frames %x to %x...", baseFrame, endFrame);
      setBitmap(framesInUse, baseFrame + 1, length - 1, 0);
    }
  }

  // Allocate pages for memory bitmap
  int memoryMapStartFrame = (int)framesInUse / PAGE_SIZE;
  int memoryMapFrames  = (bitmap_size / PAGE_SIZE + 1);
  INFO("Reserving frames %x to %x for memory bitmap...", memoryMapStartFrame, memoryMapFrames);
  setBitmap(framesInUse, memoryMapStartFrame, memoryMapFrames, 1);

  // Allocate pages in use by the kernel
  setBitmap(framesInUse, 5, 3, 1);
  setBitmap(framesInUse, 50, 3, 16);

  // The memory bitmap now resides in memory at the top of the address space
  // and is initalized with only the frames reserved by the BIOS, and the frames
  // containing the memory bitmap itself.
}

// Allocate count frames in the bit and return the address of the first frame
void *getFrames(int count) {
  INFO("Searching for %d contigous frames", count);
  int index = findContigous(framesInUse, count, frames);
  INFO("Found %d", index);
  if (index < 0)
    return NULL;

  setBitmap(framesInUse, index, count, 1);
  return (void *)(index * PAGE_SIZE);
}

void printFrames() {
  for (int f = 0; f < frames; f++) {
    INFO("%d", getBitmap(framesInUse, f));
  }
  INFO("Dumped %d frames", frames);
}
