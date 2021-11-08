OBJS = bootloader.o \
       elf.o \
       fat/fat.o \
       fat/load.o \
       hw/drive.o \
       protected/enter_protected.o \
       protected/gdt.o \
       protected/protected_init.o \

ifeq (${DEBUG}, 1)
OBJS += debug/print.o \
        debug/print_hex.o
ASFLAGS = --defsym DEBUG=1
endif

mbr.o: bootloader.ld ${OBJS}
	${LD} -T $< -o $@ ${OBJS}

.PHONY: clean
clean:
	rm -f *.o
	rm -f fat/*.o
	rm -f hw/*.o
	rm -f protected/*.o
	rm -f debug/*.o
