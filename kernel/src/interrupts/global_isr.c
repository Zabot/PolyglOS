#include <stdint.h>

#include "io/log.h"

void handlePageFault(void *address, uint32_t errorCode) {
  if (!(errorCode & 0x0004)) {
    // We should never be accessing memory addresses that weren't preallocated
    // in the kernel, so this is an error.
    PANIC("Page fault while running in kernel mode. "
          "Something is probably very wrong.");
  }

  ERROR("[Page Fault] %s 0x%x in %s mode"
      "%s"
      "%s"
      "%s"
      "%s"
      "%s"
      "%s",
      errorCode & 0x0002 ? "writing" : "reading",
      address,
      errorCode & 0x0004 ? "user" : "kernel",
      errorCode & 0x0001 ? "\n  protection violation" : "\n  page not present",
      errorCode & 0x0008 ? "\n  invalid page table" : "",
      errorCode & 0x0010 ? "\n  executing non-executable page" : "",
      errorCode & 0x0020 ? "\n  protection key violation" : "",
      errorCode & 0x0040 ? "\n  shadow stack access" : "",
      errorCode & 0x8000 ? "\n  sgx violation" : "");

  PANIC("Page faults not handled yet");
}

void handleGeneralProtectionFault(uint32_t segmentIndex) {
  PANIC("General protection fault occured: %x", segmentIndex);
}

void globalISR(uint32_t vector) {
  INFO("Handled an interrupt: %d", vector);
}
