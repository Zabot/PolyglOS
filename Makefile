all: polyglos.img

.PHONY: userspace/hello/hello
userspace/hello/hello:
	$(MAKE) -C userspace/hello

.PHONY: kernel/kernel.elf
kernel/kernel.elf: userspace/hello/hello
	$(MAKE) -C kernel

.PHONY: bootloader/bios/mbr.o
bootloader/bios/mbr.o:
	$(MAKE) -C bootloader/bios

# Create the FAT16 rootfs and copy the kernel onto it
rootfs.img: kernel/kernel.elf
	dd if=/dev/zero of=$@ bs=512 count=64k
	mkfs.fat -F 16 -n POLYGLOS -f 2 $@
	mcopy -i $@ kernel/kernel.elf ::/kernel.elf

# Create a bootable disk image by combining the mbr and rootfs and then setting
# up the partition table
polyglos.img: bootloader/bios/mbr.o rootfs.img
	dd if=bootloader/bios/mbr.o of=$@ seek=0 bs=512
	dd conv=notrunc if=rootfs.img of=$@ seek=1 bs=512
	parted --script $@ \
	    mkpart primary fat16 1s 100% \
	    set 1 boot on

.PHONY: clean
clean:
	$(MAKE) -C bootloader/bios clean
	$(MAKE) -C kernel clean
	$(MAKE) -C userspace/hello clean
	$(MAKE) -C userspace/libc clean
	rm -f *.img
