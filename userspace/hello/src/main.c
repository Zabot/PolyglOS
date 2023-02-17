#include <syscall.h>

void _start() {
  syscall(0xDEADBEEF);
  while(1);
}
