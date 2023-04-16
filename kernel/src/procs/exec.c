#include "io/log.h"
#include "procs/elf.h"

#include "memory/frames.h"
#include "memory/page_directory.h"
#include "memory/paging.h"

#include <stddef.h>

void switchUsermode(void *address);

// Given a memory address pointing to an elf, extract the elf into a new address
// space, setup the stack and bss, and schedule the program.
int exec(void *elf) {
  // Create a new address space for this process
  PageDirectory *pageDirectory = getFrames(1);
  if (pageDirectory == NULL)
    return -1;

  if (initPageDirectory(pageDirectory))
    return -1;

  usePageDirectory(pageDirectory);

  // Copy the elf sections into the address space
  struct ELFHeader *header = (struct ELFHeader *)elf;
  if (header->ident.magic != 0x464c457f) {
    PANIC("Loading elf with invalid magic number 0x%x @ 0x%x", header->ident.magic, elf);
  }
  INFO("Loading elf containing %d sections and %d segments...", header->shnum,
       header->phnum);

  struct ProgramHeader *ph = (struct ProgramHeader *)(elf + header->phoff);
  for (int i = 0; i < header->phnum; i++) {
    uint8_t *start = (uint8_t *)(elf + ph[i].offset);
    uint8_t *vaddr = (uint8_t *)(ph[i].vaddr);

    INFO("mapping %d bytes from 0x%x to 0x%x offset %x", ph[i].filesz, start,
         vaddr);
    for (int b = 0; b < ph[i].filesz; b++) {
      vaddr[b] = start[b];
    }
  }

  // Drop to user mode and exec
  void *entry = header->entry;
  INFO("elf entry point: 0x%x", entry);
  switchUsermode(entry);
}
