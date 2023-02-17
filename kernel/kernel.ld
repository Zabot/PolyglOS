ENTRY(main)

INCLUDE ../bootloader/bios/memory.ld

SECTIONS {
  . : {
    /* This section contains all of the input */
    *(*)
    user_proc = .;
    INCLUDE "hello.ld" ;
  } > protected
}
