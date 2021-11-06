all: polyglos.img

kernel/kernel.elf:
	$(MAKE) -C kernel

bootloader/bios/mbr.o:
	$(MAKE) -C bootloader/bios

# Create the FAT16 rootfs and copy the kernel onto it
rootfs.img: kernel/kernel.elf Makefile
	dd if=/dev/zero of=rootfs.img bs=512 count=64k
	mkfs.fat -F 16 -n POLYGLOS -f 2 rootfs.img
	mcopy -i rootfs.img kernel/kernel.elf ::/kernel.elf

# Create a bootable disk image by combining the mbr and rootfs and then setting
# up the partition table
polyglos.img: bootloader/bios/mbr.o rootfs.img
	dd if=bootloader/bios/mbr.o of=polyglos.img seek=0 bs=512
	dd conv=notrunc if=rootfs.img of=polyglos.img seek=1 bs=512
	parted --script ./polyglos.img \
	    mkpart primary fat16 1s 100% \
	    set 1 boot on

clean:
	$(MAKE) -C bootloader/bios clean
	$(MAKE) -C kernel clean
	rm -f *.img
