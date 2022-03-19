.code16

# Read the memory map into memoryMap
.global probe_memory
probe_memory:
  push %edx

  mov $memoryMap, %di
  xor %ebx, %ebx
  mov %ebx, %es
  mov $0x534D4150, %edx

  read_entry:
    mov $0xE820, %eax
    mov $24, %ecx
    int $0x15

    jc done
    test %ebx, %ebx
    jz done

    add $24, %di
    jmp read_entry

  done:
    mov %di, (memoryMapLength)

  pop %edx
