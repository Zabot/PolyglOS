#include "memory/paging.h"
#include "memory/page_directory.h"

#include <stddef.h>
#include <stdint.h>

#include "memory/frames.h"
#include "io/log.h"
#include "config.h"

// Defined in paging_asm.s
void switchAddressSpace(PageDirectory*);
void activatePaging();

PageDirectory *activePageDirectory;

void usePageDirectory(PageDirectory *directory) {
  switchAddressSpace(directory);

  if (activePageDirectory == NULL)
    activatePaging();

  activePageDirectory = directory;
}

int initPaging() {
  // Allocate an empty page directory
  PageDirectory *pageDirectory = getFrames(1);
  if (pageDirectory == NULL)
    return -1;

  if (initPageDirectory(pageDirectory))
    return -1;

  displayPageDirectory(pageDirectory);
  usePageDirectory(pageDirectory);
  activatePaging();
  return 0;
}

int allocatePage(void *virtAddress) {
    void *newFrame = getFrames(1);
    mapPage(activePageDirectory, virtAddress, newFrame, PT_USER | PT_READ_WRITE);
}
