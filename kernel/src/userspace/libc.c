#include "userspace/libc.h"

void do_syscall(unsigned int opcode);

void syscall(int opcode) {
  do_syscall(opcode);
}
