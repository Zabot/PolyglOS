# The MBR is the first 512 bytes on the disk. During boot the BIOS will load the
# contents of the MBR into physical memory at address 0x7C00. Then it will jump
# to 0x7C00.

# Tell as to compile this file in 16 bit mode since protected mode won't be
# enabled at first
.code16

# Initialize the state of the CPU. This is the entrypoint after boot.
boot:
  # We have to disable interrupts so that we don't get interrupted in a
  # partially initalized state
  cli

  # Initialize the segment registers that we use (we can't initalize these with
  # instant values)
  mov $0, %ax
  mov %ax, %ss
  mov %ax, %ds

  # Initialize the stack and current frame
  mov $stack_bottom, %sp
  mov %sp, %bp

  # Done with initialization, turn interrupts back on and jump to the start of
  # the bootloader
  sti
  jmp main

# The bootloader needs to do three things:
# - Load the kernel into memory
# - Setup the GDT
# - Enable protected mode
main:
  mov $initalized_string, %si
  call print_string

  # Disable interrupts to halt forever
  cli
  hlt

initalized_string: .asciz "Initialized bootloader!\r\n"
