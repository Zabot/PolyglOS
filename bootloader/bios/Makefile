all: mbr.o

OBJS = bootloader.o \
       elf.o \
       fat/fat.o \
       fat/load.o \
       hw/drive.o \
       protected/enter_protected.o \
       protected/gdt.o \
       protected/protected_init.o \
       utils/print.o \
       utils/print_hex.o

ASFLAGS = --defsym DEBUG=1

mbr.o: bootloader.ld ${OBJS}
	${LD} -T bootloader.ld -o mbr.o ${OBJS}

clean:
	rm -f *.o
	rm -f fat/*.o
	rm -f hw/*.o
	rm -f protected/*.o
	rm -f utils/*.o
