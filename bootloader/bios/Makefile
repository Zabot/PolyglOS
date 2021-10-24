all: zos.img

OBJS = bootloader.o \
       fat/fat.o \
       fat/load.o \
       hw/drive.o \
       utils/print.o \
       utils/print_hex.o

ASFLAGS = --defsym DEBUG=1

.s.o:
	as ${ASFLAGS} -o $@ $<

mbr.o: bootloader.ld ${OBJS}
	ld -T bootloader.ld -o mbr.o ${OBJS}

# Kernel is just 4 random bytes for now so we can verify if we loaded it
# correctly
kernel:
	dd if=/dev/urandom of=kernel bs=1 count=4
	dd if=/dev/zero of=kernel bs=1024 count=15 seek=1

# Create a 32MiB FAT16 partition and copy the 'kernel' onto it
partition.img: kernel
	dd if=/dev/zero of=partition.img bs=512 count=64k
	mkfs.fat -F 16 -n POLYGLOS -f 2 partition.img
	mcopy -i partition.img kernel ::/kernel.not
	mcopy -i partition.img kernel ::/kernel.bin

# Combine our partition with the MBR
zos.img: mbr.o partition.img
	dd if=/dev/zero of=zos.img bs=512 count=128
	dd conv=notrunc if=mbr.o of=zos.img seek=0 bs=512 count=8
	dd conv=notrunc if=partition.img of=zos.img seek=1 bs=512 count=64k

clean:
	rm -f kernel
	rm -f *.o *.img
	rm -f fat/*.o
	rm -f hw/*.o
	rm -f utils/*.o
