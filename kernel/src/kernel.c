#include <stdint.h>

#include "io/log.h"
#include "io/format.h"
#include "io/print.h"
#include "memory/frames.h"
#include "memory/heap.h"
#include "memory/paging.h"
#include "config.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "procs/procs.h"

int main() {
  clear();
  INFO("Booting PolyglOS");
  printMemory();
  initalizeFrameBitmap();
  initalizeHeap(HEAP_SIZE);

  if(installInterrupts()) {
    PANIC("failed to install interrupts");
  }

  // Update the GDT with a TSS and user mode segments
  initalizeGDT(1);

  if(initPaging()) {
    PANIC("failed to initialize paging");
  }

  // TODO Initalize stack (its still where it was in the bootloader)

  uint32_t *unmappedPage = (void*)0xDEADBEEF;
  INFO("should fault: %d", *unmappedPage);

  INFO("Starting userspace...");
  run(1);
}
