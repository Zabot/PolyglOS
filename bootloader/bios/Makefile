all: mbr.img

mbr.o: mbr.s print.s gdt.s

mbr.img: mbr.ld mbr.o
	ld -T mbr.ld

clean:
	rm -f *.o *.img
