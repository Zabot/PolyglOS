OBJS = src/libc.o \
			 src/do_syscall.o

CFLAGS = -ffreestanding -Iinclude

libc.a: ${OBJS}
	${LD} -o $@ ${OBJS} -shared -static

.PHONY: clean
clean:
	rm -f src/**/*.o
	rm -f *.a
