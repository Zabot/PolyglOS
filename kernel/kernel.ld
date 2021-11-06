ENTRY(main)

/*
 * Map of memory availabe when running in real mode as per
 * https://wiki.osdev.org/Memory_Map_(x86)
 */
MEMORY {
  protected (RX) : ORIGIN = 0x00010000, LENGTH = 0x00010000
  vga       (RW) : ORIGIN = 0x000A0000, LENGTH = 0x00020000
}

/* Create a symbol at the bottom of the stack for initalization */
/*stack_bottom = ORIGIN(stack) + LENGTH(stack);*/
/*m_vbr = ORIGIN(vbr);*/
/*s_vbr = ORIGIN(vbr) / 0x10;*/

m_vga = ORIGIN(vga);
/*s_root = ORIGIN(root) / 0x10;*/

/*m_fat = ORIGIN(fat);*/
/*s_fat = ORIGIN(fat) / 0x10;*/

/*m_kernel = ORIGIN(kernel);*/
/*s_kernel = ORIGIN(kernel) / 0x10;*/

/*startOfFAT         = ORIGIN(globals) + 0x00;*/
/*startOfRoot        = ORIGIN(globals) + 0x02;*/
/*startOfData        = ORIGIN(globals) + 0x04;*/
/*sectorsPerCluster  = ORIGIN(globals) + 0x06;*/
/*segmentsPerCluster = ORIGIN(globals) + 0x08;*/
/*segmentsPerCluster = ORIGIN(globals) + 0x0a;*/

SECTIONS {
  . : {
    /* This section contains all of the input */
    *(*)
  } > protected
}
