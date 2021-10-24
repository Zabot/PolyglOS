OUTPUT_FORMAT(binary)

/*
 * Map of memory availabe when running in real mode as per
 * https://wiki.osdev.org/Memory_Map_(x86)
 */
MEMORY {
  ivt     (RX) : ORIGIN = 0x00000000, LENGTH = 0x00000400
  bda     (R)  : ORIGIN = 0x00000400, LENGTH = 0x00000100

  stack   (RW) : ORIGIN = 0x00005B00, LENGTH = 0x00002000
  mbr     (RX) : ORIGIN = 0x00007C00, LENGTH = 0x00000200

  ebda    (R)  : ORIGIN = 0x00080000, LENGTH = 0x00020000
  vga     (RW) : ORIGIN = 0x000A0000, LENGTH = 0x00020000
}

/* Create a symbol at the bottom of the stack for initalization */
stack_bottom = ORIGIN(stack) + LENGTH(stack);

SECTIONS {
  . : {
    /* This section contains all of the input */
    *(*)

    /* Set the boot signature */
    . = 0x1FE;
    SHORT(0xAA55)
  } >mbr =0x00 /* Place this section in the mbr region
                * and fill any holes with 0 */
}
