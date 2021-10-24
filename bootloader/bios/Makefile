all: mbr.o

OBJS = bootloader.o \
       utils/print.o \
       utils/print_hex.o

mbr.o: bootloader.ld ${OBJS}
	ld -T bootloader.ld -o mbr.o ${OBJS}

clean:
	rm -f *.o
	rm -f utils/*.o
