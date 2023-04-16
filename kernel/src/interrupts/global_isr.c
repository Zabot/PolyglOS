#include <stdint.h>

#include "memory/paging.h"

#include "io/log.h"
#include "io/print.h"

void handlePageFault(void *address, uint32_t errorCode) {
  ERROR("[Page Fault] %s 0x%x in %s mode"
        "%s"
        "%s"
        "%s"
        "%s"
        "%s"
        "%s",
        errorCode & 0x0002 ? "writing" : "reading", address,
        errorCode & 0x0004 ? "user" : "kernel",
        errorCode & 0x0001 ? "\n  protection violation"
                           : "\n  page not present",
        errorCode & 0x0008 ? "\n  invalid page table" : "",
        errorCode & 0x0010 ? "\n  executing non-executable page" : "",
        errorCode & 0x0020 ? "\n  protection key violation" : "",
        errorCode & 0x0040 ? "\n  shadow stack access" : "",
        errorCode & 0x8000 ? "\n  sgx violation" : "");

  // Handle demand paging if the page being requested is not present
  if (!(errorCode & 0x0001)) {
    if (allocatePage(address))
      PANIC("failed to allocate page during page fault");
    return;
  }

  PANIC("unhandled page fault");
}

void handleGeneralProtectionFault(uint32_t segmentIndex) {
  PANIC("General protection fault occured: %x", segmentIndex);
}

void handleDoubleFault(uint32_t errorCode) {
  PANIC("double fault occured: %x", errorCode);
}

void handleSyscall(uint32_t syscall, uint32_t arg1) {
  if (syscall == 0x1000) {
    print((const char *)arg1);
  } else {
    PANIC("Unknown syscall 0x%x", syscall);
  }
}

void globalISR(uint32_t vector) { PANIC("Handled an interrupt: %d", vector); }
