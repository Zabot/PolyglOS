.code16

.global open_fat
open_fat:
  # Load the first block into memory
  # Get the address of the first block from partition table
  mov $0x0001, %ax # 1 block
  mov $s_vbr, %di
  mov $0x0001, %cx # From sector 1 (TODO: Read partition table)
  call read_sector
  mov $m_vbr, %bx

.ifdef DEBUG_FAT
  mov $read_vbr, %si
  call print_string
.endif

.ifdef VALIDATE_FAT_SIG
  # Verify the FAT disk signature is either 0x28 or 0x29 by shifting the LSB
  # off the end and comparing whats left
  # 0x11000 (28)
  # 0x11001 (29)
  movb 0x26(%bx), %al
  shr %al
  cmp %al, $0b00001100
  jne invalid_fat
.endif

  # Now we can start pulling information out of the BIOS Parameter Block to
  # calculate the extents of the FAT filesystem

  # How many sectors per cluster
  mov 0x0d(%bx), %al
  mov %al, (sectorsPerCluster)
  shl $5, %ax # Shift left 5 multiplies by the size of a sector (0x200), and then divides by the size of a segment (0x10)
  mov %ax, (segmentsPerCluster)

  # Calculate the start of the FAT
  addw 0x0E(%bx), %cx    # Skip the number of reserved sectors before first FAT
  mov %cx, (startOfFAT)

  # Multiply clobbers dx
  push %dx

  # Calculate start of root directory
  movb 0x10(%bx), %al   # Number of file allocation tables on device
  mulw 0x16(%bx)        # Multiply the number of FATs by the sectors per FAT
  add %cx, %ax          # Add the sectors to the start of the FAT
  mov %ax, (startOfRoot)

  # Calculate start of data directory
.ifdef CALC_ROOT
  mov 0x11(%bx), %ax  # Number of entries in the root directory
  shr $3              # Multiply by 32 bytes per entry ( << 5) and then divide by the size of a sector ( >> 8)
.else
  add $0x0020, %ax  # Assume the size
.endif
  mov %ax, (startOfData)

  pop %dx

  # Now that we know the extents, we can start loading the filesystem constructs
  # into memory

  # FAT16 will always be 128kb, so we can't read it in a single segment
  mov $s_fat, %di
  mov %di, %fs  # We don't have to set fs here for reading the FAT into memory,
                # but we will eventually need to read memory in the f segment,
                # so we might as well do it now while we have a free register
                # set to the right value

  # Only read half the FAT and hope that the kernel is in the first half.
  # Reading the other half of the FAT is pretty easy, but accessing the FAT
  # becomes more complex when we have to worry about segments
  mov 0x16(%bx), %ax  # Number of sectors per FAT
  shr %ax             # Divide by 2
  call read_sector

.ifdef READ_ENTIRE_FAT
  # Advance the read sector to the other half of the FAT and update the segment
  add %ax, %cx
  add $0x2000, %di # We're assuming the FAT is 128kb
  call read_sector
.endif

.ifdef DEBUG_FAT
  mov $read_fat, %si
  call print_string
.endif

  # Invariant: The FAT is loaded into memory at address fs:0

  # Read the root directory into memory
  mov (startOfRoot), %cx
  mov $0x20, %ax  # Assuming root directory is 20 sectors
  mov $s_root, %di # Offset  0x8000
  call read_sector

.ifdef DEBUG_FAT
  mov $read_root, %si
  call print_string
.endif

  ret


# Precondition: The FAT is loaded into memory at address %fs:0
#               The root directory is loaded into memory at %di:0
.global load_kernel
load_kernel:
  # Find the kernel in the root directory and load it to memory
  mov $m_root, %bx
  mov $kernel_file, %di
  /*push $s_kernel*/ # TODO Pass this in as an argument
  call load_file

.ifdef DEBUG_FAT
  mov $found_kernel, %si
  call print_string
.endif

  ret

kernel_file: .ascii "KERNEL  BIN"

.ifdef DEBUG_FAT
read_vbr: .asciz "Read VBR\r\n"
read_fat: .asciz "Read FAT\r\n"
read_root: .asciz "Read root\r\n"
found_kernel: .asciz "Kernel loaded\r\n"
.endif

