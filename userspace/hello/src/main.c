#include <syscalls.h>

void _start() {
  write("Hello world!\n");
  while(1);
}
