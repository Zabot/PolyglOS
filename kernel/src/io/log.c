#include "io/log.h"

#include <stdarg.h>

#include "io/format.h"

void log(const char* file, int lineno, int level, const char* fstring, ...) {
  va_list args;
  va_start(args, fstring);

  char buffer[MAX_PRINTF_LENGTH];
  vsprintf(buffer, fstring, args);

  printf("%s:%d: %s\n", file, lineno, buffer);

  va_end(args);
}

void panic(const char* file, int lineno, const char* fstring, ...) {
  va_list args;
  va_start(args, fstring);

  char buffer[MAX_PRINTF_LENGTH];
  vsprintf(buffer, fstring, args);

  va_end(args);

  printf("----KERNEL PANIC----\n"
         "%s:%d\n"
         "%s\n",
         file,
         lineno,
         buffer);
  // TODO Dump kernel state

  // Hand forever
  while(1);
}
