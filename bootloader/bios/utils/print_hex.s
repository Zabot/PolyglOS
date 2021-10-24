.code16

# Prints the contents of %dx to the screen
.global print_hex
print_hex:
	push %si

	# Print out the prefix
  mov $hex_prefix, %si
	call print_string

	# Initalize the counter to 4
	mov $4, %cx

	hex_loop:
		# Use the bottom 4 bits of dx to index a letter out of hex_table and print
		# it to the screen
		rol $4, %dx
		mov %dx, %si
		and $0x000F, %si
		movb hex_table(%si), %al
		call print_char
		dec %cx

		jcxz done
		jmp hex_loop

	done:
		mov $newline, %si
		call print_string

		pop %si
		ret

hex_table: .ascii "0123456789ABCDEF"
hex_prefix: .asciz "0x"
