.text

# We can't call lidt from c, so we have to drop to assembly to select the
# interrupt table
.global load_idt
load_idt:
	push %ebp
	mov %esp, %ebp

	mov 8(%esp), %eax
	lidt (%eax)

	mov %ebp, %esp
	pop %ebp

	ret

.macro isr vec, name
.global \name
\name:
	# Preserve the state of the machine before we jump to the isr
	pusha
	cli
	cld

	# Do the call
	mov $\vec, %eax
	push %eax
	call globalISR
	pop %eax

	# Return
	popa
	iret
.endm


ISR 0x00,divideByZero_ISR

