.text

.global switchAddressSpace
switchAddressSpace:
	# Conform to the sysV ABI
	push %ebp
	mov %esp, %ebp

	# Load the page directory into control register 3
	# Stack pointer - 4 is the base pointer we just pushed, stack pointer - 8 is argument 1
	mov 8(%esp), %eax
	mov %eax, %cr3

	# Return as per sysV ABI
	mov %ebp, %esp
	pop %ebp
	ret

# Activate the page directory
.global activatePaging
activatePaging:
	# Conform to the sysV ABI
	push %ebp
	mov %esp, %ebp

	# Set the paging bit
	mov %cr0, %eax
	or $0x80000001, %eax
	mov %eax, %cr0

	# Return as per sysV ABI
	mov %ebp, %esp
	pop %ebp
	ret
