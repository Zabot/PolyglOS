#include "procs/procs.h"
#include "io/log.h"

void switch_usermode(void *entrypoint);
void user_proc();

void run(pid_t pid) {
  INFO("Switching to user mode at 0x%x", user_proc);
  switch_usermode(user_proc);
}


