#include <stdint.h>

void do_syscall(uint32_t opcode, const void* arg);

void write(const char* buffer) {
  do_syscall(0x1000, buffer);
}
