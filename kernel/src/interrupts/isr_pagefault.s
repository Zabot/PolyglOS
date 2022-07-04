.global pageFault_ISR
pageFault_ISR:
	# Error code was pushed onto the stack before the ISR was called
	pop %eax

	# Prepare for the ISR
	pusha
	cli
	cld

	# Do the call
	push %eax
	mov %cr2, %eax
	push %eax
	call handlePageFault
	pop %eax

	# Pop the error code off the stack
	pop %eax

	# Return
	popa
	iret
