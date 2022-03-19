#include <stdint.h>

#include "io/format.h"
#include "io/print.h"
#include "memory.h"

int main() {
  clear();
  printf("Hello world!\n");
  printMemory();
}
