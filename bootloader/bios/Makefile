all: mbr.o

OBJS = bootloader.o utils/print.o

mbr.o: bootloader.ld ${OBJS}
	ld -T bootloader.ld ${OBJS}

clean:
	rm -f *.o
	rm -f utils/*.o
