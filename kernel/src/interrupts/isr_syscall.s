.global syscall_ISR
syscall_ISR:
  pusha

  # Arguments to the syscall are passed in
  # via eax and ebx. Push them to the stack
  # so we can read them in c.
  push %eax
  push %ebx
  call handleSyscall
  pop %ebx
  pop %eax

  popa
  iret
