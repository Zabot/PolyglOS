#include <stdint.h>

#include "io/format.h"
#include "io/print.h"
#include "memory.h"

int main() {
  clear();
  printf("Booting PolyglOS...\n");
  printMemory();
  initalizeFrameBitmap();

  for (int i = 0; i < 300; i++) {
    int frame = getFrame();
    if (i % 20 == 0)
      printf("Memory frames: %d (%x:%x)\n", frame, frame * PAGE_SIZE, frame * PAGE_SIZE + PAGE_SIZE - 1);
  }
}
