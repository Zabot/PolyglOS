OBJS = kernel.o

CFLAGS = -ffreestanding

kernel.elf: ${OBJS} kernel.ld
	${LD} -T kernel.ld -o kernel.elf ${OBJS}

clean:
	rm -f *.o
	rm -f *.elf
