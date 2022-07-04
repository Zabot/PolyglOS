#include <stdint.h>

#include "io/log.h"

void handlePageFault(void *address, uint32_t errorCode) {
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
  while(1);
}

void globalISR(uint32_t vector) {
  INFO("Handled an interrupt: %d", vector);
}
