OUTPUT_FORMAT(binary)

OUTPUT(mbr.img)

SECTIONS {
	. = 0x7c00;
	. : {
    mbr.o;
  }
}
