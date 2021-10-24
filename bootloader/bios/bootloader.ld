OUTPUT_FORMAT(binary)

OUTPUT(mbr.o)

SECTIONS {
  . = 0x7C00;
  . : {
    *(.text)

    FILL(0x00)

    /* Set the boot signature */
    . = 0x1FE;
    SHORT(0xAA55)
  }
}
