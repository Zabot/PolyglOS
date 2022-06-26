#include <stdint.h>

#include "io/format.h"

void globalISR(uint32_t vector) {
  printf("Handled an interrupt: %d\n", vector);
}
