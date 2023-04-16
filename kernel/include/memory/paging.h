#ifndef PAGING_H
#define PAGING_H

#include "memory/page_directory.h"

int initPaging();
int allocatePage(void *virtAddress);
void usePageDirectory(PageDirectory *directory);

#endif
