#include <stdint.h>

#include "io/format.h"
#include "io/print.h"
#include "memory/frames.h"
#include "memory/heap.h"

int main() {
  clear();
  printf("Booting PolyglOS...\n");
  printMemory();
  initalizeFrameBitmap();
  initalizeHeap(4096);

  int r = 10;
  int *array = (int *)malloc(r * sizeof(int));
  printf("Allocated an array of %d ints: at 0x%x\n", r, array);
  for (int i = 0; i < r; i++) {
    array[i] = i * i;
  }

  for (int i = 0; i < r; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
  free(array);
}
