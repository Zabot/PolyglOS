#ifndef FORMAT_H
#define FORMAT_H

#include <stdarg.h>

#define MAX_PRINTF_LENGTH 512

int itoa(int integer, char* buffer, int base);
void vprintf(const char* fstring, va_list args);
void printf(const char* fstring, ...);
void vsprintf(char *buffer, const char* fstring, va_list args);
void sprintf(char *buffer, const char* fstring, ...);

#endif
