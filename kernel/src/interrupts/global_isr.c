#include <stdint.h>

#include "io/log.h"

void globalISR(uint32_t vector) {
  INFO("Handled an interrupt: %d\n", vector);
}
