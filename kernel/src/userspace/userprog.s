.global user_main
user_main:
	mov $0xDEADBEEF, %eax
	int $0x80

	l:
	jmp l
