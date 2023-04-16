.global do_syscall
do_syscall:
  push %ebp
  mov %esp, %ebp

  mov 12(%esp), %eax
  mov 8(%esp), %ebx
  int $0x80

  mov %ebp, %esp
  pop %ebp
  ret
