default: hello

OBJS = src/main.o

SYSTEM_INCLUDES=../libc/include

CFLAGS = -ffreestanding -Iinclude -isystem ${SYSTEM_INCLUDES}

../libc/libc.a:
	$(MAKE) -C ../libc

hello: ${OBJS} ../libc/libc.a
	${LD} -o $@ ${OBJS} -static -lc -L ../libc

.PHONY: clean
clean:
	rm -f src/*.o
	rm -f hello
