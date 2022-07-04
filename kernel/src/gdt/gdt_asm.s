.global load_gdt
load_gdt:
  push %ebp
  mov %esp, %ebp

  mov 8(%esp), %eax
  lgdt (%eax)

  mov %ebp, %esp
  pop %ebp
  ret

.global load_tss
load_tss:
  push %ebp
  mov %esp, %ebp

  mov 8(%esp), %eax
  ltr %ax

  mov %ebp, %esp
  pop %ebp
  ret
