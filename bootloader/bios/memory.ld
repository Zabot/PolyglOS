/*
 * Map of memory availabe when running in real mode as per
 * https://wiki.osdev.org/Memory_Map_(x86)
 */
MEMORY {
  ivt     (RX) : ORIGIN = 0x00000000, LENGTH = 0x00000400
  bda     (R)  : ORIGIN = 0x00000400, LENGTH = 0x00000100
  globals (R)  : ORIGIN = 0x00000500, LENGTH = 0x00000100
  root    (RW) : ORIGIN = 0x00000600, LENGTH = 0x00004000
  video   (RW) : ORIGIN = 0x00000500, LENGTH = 0x00000100
  /*unused     : ORIGIN = 0x00004600, LENGTH = 0x00001300*/
  stack   (RW) : ORIGIN = 0x00005B00, LENGTH = 0x00002000
  mbr     (RX) : ORIGIN = 0x00007C00, LENGTH = 0x00000200
  vbr     (RX) : ORIGIN = 0x00007E00, LENGTH = 0x00000200
  protected  (RX) : ORIGIN = 0x00010000, LENGTH = 0x00010000
  /*unused     : ORIGIN = 0x00020000, LENGTH = 0x00030000*/
  kernel  (RX) : ORIGIN = 0x00050000, LENGTH = 0x00010000
  fat     (R)  : ORIGIN = 0x00060000, LENGTH = 0x00020000
  ebda    (R)  : ORIGIN = 0x00080000, LENGTH = 0x00020000
  vga     (RW) : ORIGIN = 0x000A0000, LENGTH = 0x00020000
}

/* Create a symbol at the bottom of the stack for initalization */
stack_bottom = ORIGIN(stack) + LENGTH(stack);
m_vbr = ORIGIN(vbr);
s_vbr = ORIGIN(vbr) / 0x10;

m_root = ORIGIN(root);
s_root = ORIGIN(root) / 0x10;

m_fat = ORIGIN(fat);
s_fat = ORIGIN(fat) / 0x10;

m_kernel = ORIGIN(kernel);
s_kernel = ORIGIN(kernel) / 0x10;

m_protected = ORIGIN(protected);
s_protected = ORIGIN(protected) / 0x10;

m_video = ORIGIN(video);
m_vga = ORIGIN(vga);

startOfFAT         = ORIGIN(globals) + 0x00;
startOfRoot        = ORIGIN(globals) + 0x02;
startOfData        = ORIGIN(globals) + 0x04;
sectorsPerCluster  = ORIGIN(globals) + 0x06;
segmentsPerCluster = ORIGIN(globals) + 0x08;
segmentsPerCluster = ORIGIN(globals) + 0x0a;
entryPoint         = ORIGIN(globals) + 0x0c;
