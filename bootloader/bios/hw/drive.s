.code16

# https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)

# Request sectors from a disk by LBA
# %ax number of sectors to read
# %bx destination address in memory
# %cx sector to read
# %dl drive to read
# %di segment to read to
.global read_sector
read_sector:
.ifdef TEST_LBA_SUPPORT
  # We want to persist these register through the lba test so we can use them
  # later
  pusha # Push ax, bx, cx

  # Test if LBA addressing is supported
  mov $0x41, %ah
  mov $0x55AA, %bx
  int $0x13
  jc lba_unsupported

  popa # Pop ax, bx, cx
.endif

  push %si

  # Since we aren't in protected mode yet, we can directly write to the packet
  # below.
  mov $lba_packet, %si # Set our base index to offset into the packet
  movw %ax, 2(%si)  # Set the number of sectors to read
  movw %di, 6(%si)  # Set the destination segment
  movw %cx, 8(%si)  # Set the first sector to read

.ifdef DEBUG_DISK
  pusha
  mov $sector_debug_msg, %si
  call print_string
  mov %cx, %dx
  call print_hex
  mov %di, %dx
  call print_hex
  popa
.endif

  # This BIOS call also uses the offset of the packet in %si
  mov $0x42, %ah
  int $0x13
  jc read_failed

  pop %si

  ret

.ifdef TEST_LBA_SUPPORT
lba_unsupported_text: .asciz "LBA addressing is unsupported\r\n"
lba_unsupported:
  mov $lba_unsupported_text, %si
  call print_string
  cli
  hlt
.endif

read_failed:
.ifdef DEBUG_DISK
  mov %ah, %dl
  mov $sector_read_error, %si
  call print_string
  call print_hex
.endif

  cli
  hlt

.ifdef DEBUG_DISK
  sector_read_error: .asciz "Failed to read sectors: "
  sector_debug_msg: .asciz "Reading sector "
.endif

# TODO If we wanted to save some bytes, we could generate this packet
# dynamically in memory since most of it is zero.
lba_packet:
  .byte 0x10        # Size of packet
  .byte 0x00        # Always 0
  .word 0x0000      # Number of sectors to transfer
  .word 0x0000      # Destination memory offset
  .word 0x0000      # Destination memory segment
  .long 0x00000000  # Lower 32-bits of 48-bit LBA
  .long 0x00000000  # Upper 16-bits of 48-bit LBA

