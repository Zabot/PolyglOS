#ifndef ELF_H
#define ELF_H

#include <stdint.h>

struct ELFIdent {
  uint32_t magic;
  uint8_t class;
  uint8_t data;
  uint8_t version;
  uint8_t osabi;
  uint8_t __padding__[8];
};

struct ELFHeader {
  struct ELFIdent ident;
  uint16_t type;
  uint16_t machine;
  uint32_t version;
  void *entry;
  uint32_t phoff;
  uint32_t shoff;
  uint32_t flags;
  uint16_t ehsize;
  uint16_t phentsize;
  uint16_t phnum;
  uint16_t shentsize;
  uint16_t shnum;
  uint16_t shstrndx;
};

struct ProgramHeader {
  uint32_t type;
  uint32_t offset;
  uint32_t vaddr;
  uint32_t paddr;
  uint32_t filesz;
  uint32_t memsz;
  uint32_t flags;
  uint32_t align;
};

struct SectionHeader {
  uint32_t name;
  uint32_t type;
  uint32_t flags;
  uint32_t addr;
  uint32_t offset;
  uint32_t size;
  uint32_t link;
  uint32_t info;
  uint32_t addralign;
  uint32_t entsize;
};

#endif
