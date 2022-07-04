#include "procs/procs.h"

void switch_usermode(void *entrypoint);
void user_main();

void run(pid_t pid) {
  switch_usermode(user_main);
}


