.code16

# Print the character in %al to the screen
.global print_char
print_char:
  # Call bios function to print at cursor
  mov $0x0E, %ah
  int $0x10

  ret

# Print the string pointed to by %si to the screen
.global print_string
print_string:
  push %si

  # Read the next character from memory and return if its zero
  print_string_loop:
    movb (%si), %al
    cmpb $0, %al
    je print_string_return

    # Print the character and loop for the next one
		call print_char
    inc %si
    jmp print_string_loop

  print_string_return:
    pop %si
    ret

.global newline
newline: .asciz "\n\r"
