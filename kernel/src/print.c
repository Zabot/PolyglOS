#include "print.h"

#include <stdint.h>

#include "port_io.h"


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

const int width = 80;

int cursor = 0;
void print(const char* string) {
  unsigned short *m_vga = (void *)0x000B8000;

  int i = 0;
  while (string[i] != '\0') {
    switch (string[i]) {
    case '\n':
      cursor += width - (cursor % width);
      break;
    default:
      m_vga[cursor] = 0x0700 | string[i];
      cursor++;
      break;
    }
    i++;
  }
  setCursor(cursor);
}
