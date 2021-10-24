all: mbr.o

OBJS = bootloader.o \
       hw/drive.o \
       utils/print.o \
       utils/print_hex.o

ASFLAGS = --defsym DEBUG=1

.s.o:
	as ${ASFLAGS} -o $@ $<

mbr.o: bootloader.ld ${OBJS}
	ld -T bootloader.ld -o mbr.o ${OBJS}

clean:
	rm -f *.o
	rm -f hw/*.o
	rm -f utils/*.o
