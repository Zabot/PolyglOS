.code32
# This is the first code that actually runs in 32 bit mode.

.global protected_init
protected_init:
  # We need to update all of our registers accordingly to use the
  # privileged data segment.
  mov $0x10,  %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  mov $stack_bottom, %ebp
  mov %ebp, %esp

  # Jump to our kernel
  call m_protected
  hlt
