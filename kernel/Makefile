OBJS = kernel.o

CFLAGS = -ffreestanding

kernel.elf: kernel.ld ${OBJS}
	${LD} -T $< -o $@ ${OBJS}

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.elf