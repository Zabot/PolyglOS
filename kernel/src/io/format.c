#include "io/format.h"

#include <stdint.h>
#include <stdarg.h>

#include "io/print.h"


int itoa(int integer, char* buffer, int base) {
  const char* digits = "0123456789ABCDEF";

  char negative = 0;
  if (integer < 0) {
    negative = 1;
    integer = -integer;
  }

  int i = 0;
  char reversed[64]; // Big enough to dump 64 bit numbers in binary
  if (integer == 0) {
    reversed[i++] = digits[0];
  } else {
    while (integer != 0) {
      reversed[i++] = digits[integer % base];
      integer /= base;
    }
  }

  if (negative) {
    reversed[i++] = '-';
  }

  int length = i;
  for (i = 0; i < length; i++) {
    buffer[i] = reversed[length - i - 1];
  }

  return length;
}

void printf(const char* fstring, ...) {
  char buffer[MAX_PRINTF_LENGTH];

  va_list args;
  va_start(args, fstring);

  int o = 0;
  int i = 0;
  while (fstring[i] != '\0') {
    if (fstring[i] == '%') {
      i++;
      switch (fstring[i]) {
        case 'b':
          o += itoa(va_arg(args, int), buffer + o, 2);
          break;
        case 'x':
          o += itoa(va_arg(args, uint64_t), buffer + o, 16);
          break;
        case 'd':
          o += itoa(va_arg(args, int), buffer + o, 10);
          break;
      }
    } else {
      buffer[o] = fstring[i];
      o++;
    }
    i++;
  }

  va_end(args);
  buffer[o] = '\0';

  print(buffer);
}
