#ifndef PAGE_DIRECTORY_H
#define PAGE_DIRECTORY_H

#include <stdint.h>

#define PDE_ENTRIES 1024
#define PT_ENTRIES 1024

#define PDE_PAT 0x00001000
#define PDE_GLOBAL 0x00000100
#define PDE_PAGE_SIZE 0x00000080
#define PDE_DIRTY 0x00000040
#define PDE_ACCESSED 0x00000020
#define PDE_CACHE_DISABLE 0x00000010
#define PDE_WRITE_THROUGH 0x00000008
#define PDE_USER 0x00000004
#define PDE_READ_WRITE 0x00000002
#define PDE_PRESENT 0x00000001
#define PDE_ADDRESS 0xFFFFF000

#define PT_GLOBAL 0x00000100
#define PT_PAGE_SIZE 0x00000080
#define PT_PAT 0x00000080
#define PT_DIRTY 0x00000040
#define PT_ACCESSED 0x00000020
#define PT_CACHE_DISABLE 0x00000010
#define PT_WRITE_THROUGH 0x00000008
#define PT_USER 0x00000004
#define PT_READ_WRITE 0x00000002
#define PT_PRESENT 0x00000001
#define PT_ADDRESS 0xFFFFF000

typedef uint32_t PageTableEntry;
typedef PageTableEntry PageTable[PT_ENTRIES];

typedef uint32_t PageDirectoryEntry;
typedef PageDirectoryEntry PageDirectory[PDE_ENTRIES];

int mapPage(PageDirectory *directory, void *virtualAddress,
            void *physicalAddress, uint32_t flags);
int initPageDirectory(PageDirectory *directory);
void displayPageDirectory(PageDirectory *directory);

#endif
