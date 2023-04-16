OBJS = src/do_syscall.o src/syscalls.o

CFLAGS = -ffreestanding -Iinclude

libc.a: ${OBJS}
	${AR} -crs $@ ${OBJS}

.PHONY: clean
clean:
	rm -f **/*.o
	rm -f *.a
