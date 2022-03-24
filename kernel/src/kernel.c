#include <stdint.h>

#include "io/format.h"
#include "io/print.h"
#include "memory/frames.h"

int main() {
  clear();
  printf("Booting PolyglOS...\n");
  printMemory();
  initalizeFrameBitmap();

  for (int i = 0; i < 10; i++) {
    int frame = getFrames(10);
    printf("Memory frames: %d (%x:%x)\n", frame, frame * PAGE_SIZE, frame * PAGE_SIZE + PAGE_SIZE - 1);
  }
}
