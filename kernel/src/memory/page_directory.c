#include "memory/page_directory.h"

#include <stddef.h>

#include "io/log.h"
#include "memory/frames.h"

#define PAGE_SIZE 4096

// extracts the referenced page table from a page directory entry
PageTable *pdeAddressRead(PageDirectoryEntry entry) {
  if (entry & PDE_PAGE_SIZE) {
    // Maps directly to a 4MiB page
    PANIC("Accessing large pages not supported");
    return NULL;
  }

  return (PageTable *)(entry & PT_ADDRESS);
}

// maps a virtual address to a physical address in the given page table, setting
// any flags along the way.
int mapPage(PageDirectory *directory, void *virtualAddress,
            void *physicalAddress, uint32_t flags) {
  // Virtual address format
  // DDDD DDDD DDTT TTTT TTTT OOOO OOOO OOOO
  uintptr_t va = (uintptr_t)virtualAddress;
  uint32_t pdeIndex = (va & 0xFFC00000) >> 22;
  uint32_t ptIndex = (va & 0x003FF000) >> 12;

  PageTable *pageTable;
  PageDirectoryEntry *pdeEntry = (*directory) + pdeIndex;
  if (*pdeEntry & PDE_PRESENT) {
    pageTable = pdeAddressRead(*pdeEntry);
  } else {
    // If the entry wasn't present in the page directory, create a new one
    pageTable = getFrames(1);
    if (pageTable == NULL) {
      return -1;
    }

    for (int t = 0; t < PT_ENTRIES; t++) {
      (*pageTable)[t] = 0;
    }
    *pdeEntry = ((uintptr_t)pageTable & PDE_ADDRESS) | PDE_PRESENT | flags;
    INFO("allocated new page table at 0x%x with flags %x", pageTable,
         PDE_PRESENT | flags);
  }

  INFO("mapped 0x%x to 0x%x with flags 0x%x",
       ((uintptr_t)virtualAddress) & PT_ADDRESS, physicalAddress,
       PT_PRESENT | flags);
  (*pageTable)[ptIndex] =
      (((uintptr_t)physicalAddress) & PT_ADDRESS) | PT_PRESENT | flags;

  return 0;
}

int initPageDirectory(PageDirectory *directory) {
  // Clear the whole directory
  for (int i = 0; i < PDE_ENTRIES; i++) {
    (*directory)[i] = PDE_READ_WRITE;
  }

  // Identity map all of the low memory (includes everything setup by
  // bootloader)
  // TODO Make this dynamic
  for (void *i = 0; i < (void *)0x000FFFFF; i += PAGE_SIZE) {
    if (mapPage(directory, i, i, PT_PRESENT | PT_READ_WRITE)) {
      PANIC("failed to identity map low memory");
      return -1;
    }
  }

  // Identity map the in use frames bitmap
  // TODO Make this dynamic
  if (mapPage(directory, (void *)0x7FDF000, (void *)0x7FDF000,
              PT_PRESENT | PT_READ_WRITE)) {
    PANIC("failed to map in use frames");
    return -1;
  }

  return 0;
}

void displayPageDirectory(PageDirectory *directory) {
  INFO("Page directory at 0x%x:", directory);
  for (int i = 0; i < PDE_ENTRIES; i++) {
    PageDirectoryEntry pdEntry = (*directory)[i];
    if (pdEntry & PDE_PRESENT) {
      PageTable *pageTable = pdeAddressRead(pdEntry);
      INFO("Entry %d (pages %d-%d) [0x%x]:", i, i * 1024, (i + 1) * 1024,
           pageTable);
      for (int e = 0; e < PT_ENTRIES; e++) {
        PageTableEntry ptEntry = (*pageTable)[e];
        if (ptEntry & PT_PRESENT) {
          INFO("  0x%x(virt):0x%x(phys)", PAGE_SIZE * (i * 1024 + e),
               ptEntry & PT_ADDRESS);
        }
      }
    }
  }
}
