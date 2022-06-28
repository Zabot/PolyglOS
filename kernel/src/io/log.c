#include "io/log.h"

#include <stdarg.h>

#include "io/format.h"

void log(const char* file, int lineno, int level, const char* system, const char* fstring, ...) {
  va_list args;
  va_start(args, fstring);

  char buffer[MAX_PRINTF_LENGTH];
  vsprintf(buffer, fstring, args);

  printf("[%s@%s:%d]: %s\n", system, file, lineno, buffer);

  va_end(args);
}
