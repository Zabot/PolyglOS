.global syscall_ISR
syscall_ISR:
	# Prepare for the ISR
	pusha
	cld

	# Do the call
	push %eax
	call handleSyscall
	pop %eax

	# Return
	popa
	iret
