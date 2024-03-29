#include "bitmap.h"

#define MASK(start, count)                                                     \
  (((1 << (count - 1)) | ((1 << count - 1) - 1))) << start

// Set length bits starting at start in map to value
void setBitmap(WORD *map, int start, int length, int value) {
  while (length > 0) {
    int maskOffset = start % WORD_SIZE;
    int maskLength = length;

    if (maskOffset + maskLength > WORD_SIZE) {
      maskLength = WORD_SIZE - maskOffset;
    }

    WORD mask = MASK(maskOffset, maskLength);
    if (value)
      map[start / WORD_SIZE] |= mask;
    else
      map[start / WORD_SIZE] &= ~mask;

    start += maskLength;
    length -= maskLength;
  }
}

// Get the value of the bitmap at _index_
int getBitmap(WORD *map, int index) {
  WORD mask = MASK(index % WORD_SIZE, 1);
  return (map[index / WORD_SIZE] & mask) != 0;
}

// Find the first set of count contigous bits
int findContigous(WORD *map, int count, int length) {
  for (int base = 0; base < length; base++) {
    int i;
    for (i = 0; i < count && base + i < length; i++) {
      if (getBitmap(map, base + i))
        break;
    }

    if (i == count)
      return base;
  }

  return -1;
}
