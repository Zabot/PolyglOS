all: zos.img

OBJS = bootloader.o \
       fat/fat.o \
       fat/load.o \
       hw/drive.o \
       utils/print.o \
       utils/print_hex.o \
       elf.o

ASFLAGS = --defsym DEBUG=1

.s.o:
	${AS} ${ASFLAGS} -o $@ $<

mbr.o: bootloader.ld ${OBJS}
	${LD} -T bootloader.ld -o mbr.o ${OBJS}

# Create a 32MiB FAT16 partition and copy the kernel onto it
partition.img: kernel.elf
	dd if=/dev/zero of=partition.img bs=512 count=64k
	mkfs.fat -F 16 -n POLYGLOS -f 2 partition.img
	mcopy -i partition.img kernel.elf ::/kernel.elf

# Combine our partition with the MBR
zos.img: mbr.o partition.img
	dd if=/dev/zero of=zos.img bs=512 count=128
	dd conv=notrunc if=mbr.o of=zos.img seek=0 bs=512 count=8
	dd conv=notrunc if=partition.img of=zos.img seek=1 bs=512 count=64k

clean:
	rm -f kernel.elf
	rm -f *.o *.img
	rm -f fat/*.o
	rm -f hw/*.o
	rm -f utils/*.o
