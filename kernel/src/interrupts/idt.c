#include "interrupts/idt.h"

#include <stddef.h>
#include <stdint.h>

#include "memory/frames.h"
#include "io/log.h"

#define IDT_SIZE 256

struct __attribute__((packed)) idtr {
  uint16_t size;
  uint32_t offset;
};

void load_idt(struct idtr*);

struct __attribute__((packed)) gateDescriptor {
    uint16_t offsetLow;
    uint16_t segment;
    uint8_t reserved;
    uint8_t flags;
    uint16_t offsetHigh;
};

#define INSTALL_INTERRUPT(vector,isr) \
  void isr();\
  idt[vector].flags = 0x8E;\
  idt[vector].offsetHigh = (uintptr_t)isr >> 16;\
  idt[vector].offsetLow = (uintptr_t)isr & 0xFFFF;\
  idt[vector].segment = 0x08;

int installInterrupts() {
  void *base = getFrames(1);
  if (base == NULL)
    return -1;

  // Make sure all of the ISRs are disabled by default
  struct gateDescriptor *idt = base + sizeof(struct idtr);
  for (int i = 0; i < IDT_SIZE; i ++)
    idt[i].flags = 0;

  INSTALL_INTERRUPT(0x00, divideByZero_ISR);
  INSTALL_INTERRUPT(0x0D, gpFault_ISR);
  INSTALL_INTERRUPT(0x0E, pageFault_ISR);
  INSTALL_INTERRUPT(0x80, syscall_ISR);
  idt[0x80].flags = 0xEE;

  struct idtr *descriptor = base;
  descriptor->offset = (uintptr_t)idt;
  descriptor->size = sizeof(struct gateDescriptor) * IDT_SIZE - 1;
  load_idt(descriptor);

  return 0;
}
