#include "print.h"

#include <stdint.h>

void outb(uint16_t port, uint8_t value)
{
      __asm("outb %b0, %w1"
                  :: "a"(value), "d"(port));
}

void setCursor(int cursor) {
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t) (cursor & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t) ((cursor >> 8) & 0xFF));
}

void clear() {
  unsigned short *m_vga = (void *)0x000B8000;

  for(int i = 0; i < 4000; i ++) {
    m_vga[i] = 0x0700 | ' ';
  }

  setCursor(0);
}

void print(const char* string) {
  unsigned short *m_vga = (void *)0x000B8000;

  static int cursor = 0;

  int i = 0;
  while (string[i] != '\0') {
    m_vga[cursor + i] = 0x0700 | string[i];
    i++;
  }
  // Advance the cursor by the length of the string.
  cursor += i;

  setCursor(cursor);
}
