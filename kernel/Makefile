OBJS = src/kernel.o \
       src/print.o \
       src/format.o \
       src/port_io.o

CFLAGS = -ffreestanding -Iinclude

kernel.elf: kernel.ld ${OBJS}
	${LD} -T $< -o $@ ${OBJS}

.PHONY: clean
clean:
	rm -f src/*.o
	rm -f *.elf
