#include "io/format.h"

#include <stdarg.h>
#include <stdint.h>

#include "io/print.h"

int utoa(unsigned int integer, char *buffer, int base) {
  const char *digits = "0123456789ABCDEF";

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

  int length = i;
  for (i = 0; i < length; i++) {
    buffer[i] = reversed[length - i - 1];
  }

  return length;
}

int itoa(int integer, char *buffer, int base) {
  int negative = 0;
  if (integer < 0) {
    buffer[0] = '-';
    integer = -integer;
  }

  return negative + utoa(integer, buffer + negative, base);
}

void vsprintf(char *buffer, const char *fstring, va_list args) {
  int o = 0;
  for (int i = 0; fstring[i] != '\0'; i++) {
    if (fstring[i] == '%') {
      i++;
      switch (fstring[i]) {
      case 'b':
        o += itoa(va_arg(args, unsigned int), buffer + o, 2);
        break;
      case 'x':
        o += utoa(va_arg(args, uint32_t), buffer + o, 16);
        break;
      case 'd':
        o += itoa(va_arg(args, int), buffer + o, 10);
        break;
      case 'l':
        // TODO Implement log format codes properly (%lx, %ld, etc)
        o += itoa(va_arg(args, uint64_t), buffer + o, 16);
        break;
      case 's': {
        char *s = va_arg(args, char *);
        for (int ii = 0; s[ii] != 0; ii++) {
          buffer[o] = s[ii];
          o++;
        }
      } break;
      }
    } else {
      buffer[o] = fstring[i];
      o++;
    }
  }

  buffer[o] = '\0';
}

void sprintf(char *buffer, const char *fstring, ...) {
  va_list args;
  va_start(args, fstring);

  vsprintf(buffer, fstring, args);
  va_end(args);
}

void vprintf(const char *fstring, va_list args) {
  char buffer[MAX_PRINTF_LENGTH];
  vsprintf(buffer, fstring, args);
  print(buffer);
}

void printf(const char *fstring, ...) {
  va_list args;
  va_start(args, fstring);

  vprintf(fstring, args);
  va_end(args);
}
