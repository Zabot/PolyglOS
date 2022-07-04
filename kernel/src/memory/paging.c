#include "memory/paging.h"

#include <stddef.h>
#include <stdint.h>

#include "memory/frames.h"
#include "io/log.h"
#include "config.h"

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

// Defined in paging_asm.s
void switchAddressSpace(uint32_t *);
void activatePaging();

void printPageMap(uint32_t *pageDirectory) {
  INFO("Page table at 0x%x:", pageDirectory);
  for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
    if (pageDirectory[i] & 0x01) {
      /*INFO("Entry %d (pages %d-%d) [0x%x]:", i, i*1024, (i+1)*1024, pageDirectory[i]);*/
      uint32_t *pageTable = (void *)(pageDirectory[i] & 0xFFFFF000);
      for (int e = 0; e < PAGE_TABLE_ENTRIES; e++) {
        if(pageTable[e] & 0x01) {
          INFO("  0x%x(virt):0x%x(phys)", PAGE_SIZE * (i * 1024 + e), pageTable[e] & 0xFFFFF000);
        }
      }
    }
  }
}

int mapFrame(uint32_t *pageDirectory, void *virtAddress, void* physAddress) {
  int page = (intptr_t)virtAddress / PAGE_SIZE;
  int index = page / PAGE_DIRECTORY_ENTRIES;

  uint32_t *pageTable;
  if (pageDirectory[index] & 0x01) {
    pageTable = (void *)(pageDirectory[index] & 0xFFFFF000);
  } else {
    pageTable = getFrames(1);
    if (pageTable == NULL) {
      return -1;
    }
    pageDirectory[index] = (uintptr_t)pageTable & 0xFFFFF000 | 0x3;
  }

  INFO("global %d", page);
  page = page % PAGE_DIRECTORY_ENTRIES;
  INFO("page %d", page);
  if (pageTable[page] & 0x01) {

  } else {
    pageTable[page] = (uintptr_t)physAddress & 0xFFFFF000 | 0x3;
  }

  return 0;
}

int identityMap(uint32_t *pageDirectory, void *address) {
  return mapFrame(pageDirectory, address, address);
}

int initPaging() {
  uint32_t *pageDirectory = getFrames(1);
  if (pageDirectory == NULL)
    return -1;

  for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i ++)
    pageDirectory[i] = 0x00000002; // Supervisor only not present

  // Map all of the memory we might care about
  // TODO Cleanup where memory is so we don't have to do this.
  INFO("Identity mapping");
  for (int i = 0; i < 0x000C0000; i+= PAGE_SIZE) {
    if (identityMap(pageDirectory, (void*)i)) {
      return -1;
    }
  }
  printPageMap(pageDirectory);

  switchAddressSpace(pageDirectory);
  activatePaging();
  return 0;
}
