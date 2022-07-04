#include "userspace/libc.h"

void user_main() {
  syscall(0xDEADBEEF);
  while(1);
}
