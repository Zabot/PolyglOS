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
#include "procs/exec.h"

// user_proc is a pointer to an ELF that is copied into the kernel
// so we can avoid trying to load it from the filesystem for now.
void user_proc();

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

  INFO("Address of user elf: %x", &user_proc);
  exec(user_proc);

  PANIC("end of main");
}
