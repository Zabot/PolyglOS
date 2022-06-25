#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

#ifdef __amd64__
#define WORD_SIZE 64
#define WORD uint64_t
#define MAX_WORD 0xFFFFFFFFFFFFFFFF
#elif __i386__
#define WORD_SIZE 32
#define WORD uint32_t
#define MAX_WORD 0xFFFFFFFF
#endif

void setBitmap(WORD *map, int start, int length, int value);
int getBitmap(WORD *map, int index);

int findContigous(WORD *map, int count, int length);

#endif
