#include <stdint.h>

#include "format.h"
#include "print.h"

int main() {
  clear();
  printf("Hello world! %d 0x%x", 123, 0x123);
}
