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

  # Jump to our kernel
  mov $stack_bottom, %ebp
  mov %ebp, %esp

  # Write some stuff to the video buffer
  mov $m_vga, %ebx
  write_video:
    mov $0x07, %ah
    mov $'W', %al
    add $2, %ebx
    mov %ax, (%ebx)
    jmp write_video

  hlt
