# The MBR is the first 512 bytes on the disk. During boot the BIOS will load the
# contents of the MBR into physical memory at address 0x7C00. Then it will jump
# to 0x7C00.

# Tell as to compile this file in 16 bit mode since protected mode won't be
# enabled at first
.code16

# Initialize the state of the CPU. This is the entrypoint after boot.
boot:
	# We have to disable interrupts so that we don't get interrupted in a
	# partially	initalized state
 	cli

  # Initialize the segment registers that we use (we can't initalize these with
	# instant values)
 	mov $0, %ax
	mov %ax, %ss
	mov %ax, %ds

	# Initialize the stack and current frame
  mov $0x7C00, %sp
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

# We have to incude here instead of linking because we need exact control over
# where the boot signature and parition table need to go.
# exactly 512 bytes
.include "print.s"

initalized_string: .asciz "Initialized bootloader!\r\n"

# The global descriptor table is an in memory structure that defines the
# addressable memory segments that can be used in protected mode.
.include "gdt.s"

# Set the 4 byte unique drive ID
.org 0x1B8
.long 0xDEADBEEF

# Mark the drive as not read-only
.org 0x1BC
.word 0x0000

# Parition table
.org 0x1BE
.org 0x1CE
.org 0x1DE
.org 0x1EE

# Boot signature
.org 0x1FE
.word 0xaa55
