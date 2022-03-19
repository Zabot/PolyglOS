OUTPUT_FORMAT(binary)

INCLUDE ./memory.ld

SECTIONS {
  . : {
    /* This section contains all of the input */
    *(*)

    /* Save room for a partition table */
    . = 0x1BE;

    /* Set the boot signature */
    . = 0x1FE;
    SHORT(0xAA55)
  } >mbr =0x00 /* Place this section in the mbr region
                * and fill any holes with 0 */
}
