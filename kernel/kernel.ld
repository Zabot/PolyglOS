ENTRY(main)

INCLUDE ../bootloader/bios/memory.ld

SECTIONS {
  . : {
    /* This section contains all of the input */
    *(*)
  } > protected
  . : {
    user_proc = .;
    ./hello.bin
  }
}
