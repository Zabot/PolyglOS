.code16

enable_a20:
  # Test if a20 is already on
  in $0x92, %al
  test $0x02, %al
  jnz a20_on

  # Turn a20 on
  or $0x02, %al
  and $0xFE, %al
  out %al, $0x92

  a20_on:
  ret

.global enter_protected
enter_protected:
  # Enable the 20th address line (due to legacy reasons this is disabled at boot)
	call enable_a20

  # Disable interrupts (Intel recommends we do this before switching to
  # protected mode)
  cli

  # Load the GDT from memory
  lgdt (gdt)

  # Enable the protected mode bit (bit 0) in the control register
  mov %cr0, %eax
  or $1, %al
  mov %eax, %cr0

  # Now that we're in protected mode, segment registers refer to the segment
  # offset in the GDT. We can't execute any more code until we change the %cs
  # to point to the correct entry in the GDT. We do this with a far jump
  #instruction (A jump instruction that also specifies a value for %cs).
  jmp $0x08, $protected_init
