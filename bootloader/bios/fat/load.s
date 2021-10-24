.code16

# Search a dictory for a file and load the file into memory
# %bx The start of the directory to search
# %di The name of the file to search for
# The destination of the search should be pushed to the stack before calling
# Preconditions:
#   (sectorsPerCluster) contains the number of sectors in a single cluster
#   (startOfData) contains the first sector number of the data segment of the filesystem
.global load_file
load_file:
  # Compare the first 11 bytes from the directory entry to the expected filename
  push %di
  mov %bx, %si
  mov $11, %cx
  repe cmpsb
  pop %di
  je preload_cluster

  # Advance to the next directory entry.
  add $0x0020, %bx

  # TODO This is an infinite loop, Make sure we don't run out of the directory
  jmp load_file

  preload_cluster:
    # TODO %di needs to be pulled off the stack before continuing
    /*pop %di*/
    mov $s_kernel, %di

    # Calculate the first sector in this cluster
    mov 0x1A(%bx), %ax  # Get the first cluster from the directory entry

  # Load the clusters of a file into a contigous block of memory
  # Preconditions:
  #   %bx points to the directory entry of the file we want to load
  #   %di points to the segment that we will load the file into
  load_cluster:
.ifdef DEBUG_FAT_LOADING
    pusha
    mov %ax, %dx
    mov $loaded_cluster, %si
    call print_string
    call print_hex
    mov %di, %dx
    call print_hex
    popa
.endif

    # Invariant: %ax is the number of the cluster we want to read
    push %ax   # Push first sector onto the stack so we can mangle it and then pop later

    # Calculate the first sector of the cluster in %cx
    sub $2, %ax               # There are two sectors at the start of the FAT that don't have data
    # TODO This multiplication is always by a power of 2, so we could save some instructions by doing
    # this math directly in cx with a shift
    imulb (sectorsPerCluster) # Calculate how many sectors past the start of the section this cluster is
    add (startOfData), %ax    # Offset into the data section
    mov %ax, %cx

    # Load the first cluster to memory
    mov (sectorsPerCluster), %ax
    call read_sector

    # Now we need to look up the next cluster in the FAT
    pop %bx             # Grab the cluster number back off the stack
    shl %bx             # Multiply by 2 to get the byte offset into the FAT
    mov %fs:(%bx), %ax  # Read the next cluster number from the FAT

    # %ax contains the index of the next cluster. If it is 0xFFF8 or greater,
    # the file has all been read
    cmp $0xFFF8, %ax
    jae loading_done

    # We aren't done with the file yet, so advance the segment and read the
    # next cluster.
    add (segmentsPerCluster), %di
    jmp load_cluster

  loading_done:
    ret

.ifdef DEBUG_FAT_LOADING
  loaded_cluster: .asciz "Loading cluster "
.endif
