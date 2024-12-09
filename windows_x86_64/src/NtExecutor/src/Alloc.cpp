#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include "Alloc.h"

#define alignSize(n) (n + ( 8 - (n % 8)))

size_t bufIdx = 0;
char allocBuf[BUF_RESERVE_LEN];

// Custom memory allocator to avoid calling syscalls that request heap memory.
void* alloc(size_t size) {
  char* p;
  int i;

  if (bufIdx + size + ALLOC_PADDING_SIZE > BUF_RESERVE_LEN) {
    printf("Failed to allocate %zd byte memory\n", size);
    exit(1);
  }
  size = alignSize(size);
  p = (allocBuf + bufIdx);
  bufIdx += (size + ALLOC_PADDING_SIZE);
  for (i = 0; i < ALLOC_PADDING_SIZE; i += sizeof(unsigned int)) {
    *(unsigned int*)(p + size + i) = 0xdeadc0de;
  }
  return p;
}
