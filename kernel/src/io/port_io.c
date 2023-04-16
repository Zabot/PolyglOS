#include "io/port_io.h"

uint8_t inb(uint16_t port) {
  // In ATT syntax the first argument is the destination operand, but GNU
  // inline assembly wants the source operands before the destination operands,
  // so we have to flip the order. inb reads the io port at %1 (either the dx
  // register or an immediate) into %0 (the a register).
  uint8_t value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

uint16_t inw(uint16_t port) {
  uint16_t value;
  asm volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

uint32_t inl(uint16_t port) {
  uint32_t value;
  asm volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void outb(uint16_t port, uint8_t value) {
  // outb had two parameters. The first parameter should be stored in the a
  // register (eax/ax/al), and the second parameter should either be stored
  // in the d register (edx/dx/dl) or in the instruction as an 8bit immediate
  // value
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(uint16_t port, uint16_t value) {
  asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

void outl(uint16_t port, uint32_t value) {
  asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}
