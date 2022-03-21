OBJS = src/kernel.o \
       src/io/port_io.o \
       src/io/print.o \
       src/io/format.o \
       src/memory.o \
       src/bitmap.o

CFLAGS = -ffreestanding -Iinclude

kernel.elf: kernel.ld ${OBJS}
	${LD} -T $< -o $@ ${OBJS}

.PHONY: clean
clean:
	rm -f src/*.o
	rm -f *.elf
