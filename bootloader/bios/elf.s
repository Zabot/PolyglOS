.code16

# Precondition: %ds:0 points to the start of an elf file
# %es:0 points to the start of the address space this program should be loaded
#       into.
.global load_elf
load_elf:
  xor %bx, %bx

.ifdef VALIDATE_ELF
  xor %si, %si

  mov $4, %cx
  mov $elf_magic, %di
  repe cmpsb
  je valid_elf

  # ELF is invalid
  mov $invalid_elf, %dx
  call print_string
.endif

valid_elf:
  mov %ds:0x2A(%bx), %ax # How big is a segment table entry
  mov %ds:0x2C(%bx), %cx # How many segment table entries are there?
  mov %ds:0x1C(%bx), %bx # Where does the segment table start

  /*mov $0x22(%bx), %ax # Where does the section header start*/
  /*movw $0x30(%bx), %cx # How many entries are in the section header*/

  # Precondition, %ds:%bx contains the address of an entry in the section table
  load_segment:
    push %cx # We're gonna clobber cx when we relocate the segment, so push it
    cmpb $0x0001, (%bx) # If the first byte isn't a 1, this section isn't loadable
    jne next_segment

    # Calculate the destination memory segment
    push %ax
    mov %ds:0x0A(%bx), %ax
    shl $12, %ax
    mov %ax, %es
    pop %ax

    mov %ds:0x04(%bx), %si # Where does this section start
    mov %ds:0x08(%bx), %di # Where does the memory go
    mov %ds:0x10(%bx), %cx # How big is the section

    rep movsb # Copy the segment from %ds:%si to %es:%di

  next_segment:
    # Advance to the next entry in the segment table
    pop %cx
    add %ax, %bx
    loop load_segment

  # At this point all of the segments have been loaded
  ret

.ifdef DEBUG_ELF
invalid_elf:
  .asciz "Invalid ELF\r\n"
.endif

.ifdef VALIDATE_ELF
elf_magic:
  .byte 0x7F
  .ascii "ELF"
.endif
