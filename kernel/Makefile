all: kernel.elf

OBJS = src/kernel.o \
       src/gdt/gdt.o \
       src/gdt/gdt_asm.o \
       src/io/port_io.o \
       src/io/print.o \
       src/io/format.o \
       src/io/log.o \
       src/memory/frames.o \
       src/memory/heap.o \
       src/memory/paging.o \
       src/memory/paging_asm.o \
       src/memory/page_directory.o \
       src/interrupts/idt.o \
       src/interrupts/isr.o \
       src/interrupts/isr_pagefault.o \
       src/interrupts/isr_gpfault.o \
       src/interrupts/isr_syscall.o \
       src/interrupts/global_isr.o \
       src/procs/exec.o \
       src/procs/enter_userspace.o \
       src/bitmap.o

CFLAGS = -ffreestanding -Iinclude

hello.ld: ../userspace/hello/hello
	cat $< | hexdump -v -e '"BYTE(0x" 1/1 "%02X" ")\n"' > $@

kernel.elf: kernel.ld hello.ld ${OBJS}
	${LD} -T $< -o $@ ${OBJS}

.PHONY: clean
clean:
	rm -f src/**/*.o
	rm -f *.elf
	rm -f hello.ld
