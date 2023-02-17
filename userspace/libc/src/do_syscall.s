.global do_syscall
do_syscall:
  push %ebp
  mov %esp, %ebp

  mov 8(%esp), %eax
	int $0x80

  mov %ebp, %esp
  pop %ebp
  ret
