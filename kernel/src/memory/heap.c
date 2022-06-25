#include "memory/heap.h"

#include "memory/frames.h"
#include "bitmap.h"

#include "io/format.h"

#define HEAP_BLOCK_SIZE 16

int heapSize;
WORD *heapBitmap;

// The actual start of the heap
void *heap;

// How many blocks of some size are needed to store size data
size_t fillBlocks (size_t size, size_t blockSize) {
  return (size / blockSize) + (size % blockSize > 0);
}

int initalizeHeap(size_t size) {
  heapSize = fillBlocks(size, HEAP_BLOCK_SIZE);
  size_t bitmapSize = fillBlocks(heapSize, WORD_SIZE);
  int heapFrames = fillBlocks(size, PAGE_SIZE);
  heapBitmap = getFrames(heapFrames);
  if (heapBitmap == NULL) {
    return 1;
  }
  setBitmap(heapBitmap, 0, heapSize, 0);
  heap = heapBitmap + bitmapSize;

  printf("Heap initalized:\n"
      "  Frames: %d\n"
      "  Usable space: %d bytes\n"
      "  Start: 0x%x\n",
      heapFrames,
      size - bitmapSize,
      heap
    );
}

struct blockHeader {
  size_t length;
};

void *malloc(size_t size) {
  int blocks = fillBlocks(size + sizeof(struct blockHeader), HEAP_BLOCK_SIZE);

  int index = findContigous(heapBitmap, blocks, heapSize);
  if (index == -1)
    return NULL;

  setBitmap(heapBitmap, index, blocks, 1);

  // [length][memory allocated]
  struct blockHeader *header = heap + (index * HEAP_BLOCK_SIZE);
  header->length = blocks;

  printf("Allocated %d blocks starting at %d\n", header->length, index);
  return (void*)header + sizeof(struct blockHeader);
}

void free(void *ptr) {
  struct blockHeader *header = ptr - sizeof(struct blockHeader);
  int index = ((void*)header - heap) / HEAP_BLOCK_SIZE;
  setBitmap(heapBitmap, index, header->length, 0);
  printf("Freed %d blocks starting at %d\n", header->length, index);
}
