OBJS = src/main.o

SYSTEM_INCLUDES=../libc/include

CFLAGS = -ffreestanding -Iinclude -isystem ${SYSTEM_INCLUDES}

hello: ${OBJS}
	${LD} -o $@ ${OBJS} -lc -L../libc/

.PHONY: clean
clean:
	rm -f src/**/*.o
	rm -f *.elf
