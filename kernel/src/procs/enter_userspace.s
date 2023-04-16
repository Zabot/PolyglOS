.global switchUsermode
switchUsermode:
	# Only argument is the usermode address to jump to
	mov 4(%esp), %edx

	# Switch all of the segment registers to the user data segment (index 4) with
	# DPL 3
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	# TODO Move the stack pointer somewhere in user address space
	mov $0x00FFFFFFF, %eax

	# Construct a stack such that iret returns to the user code segment
	push $0x23
	push %eax
	pushf
	push $0x1B
	push %edx

	iret
